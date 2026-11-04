# ============================================================================
#  多视觉融合的三维重建平台 —— 顶层 CMake 配置
#  目标平台：Windows 10/11 x64 + VS2022 + CUDA 11.8/12.x
#  依赖：OpenCV(源码编译, 含 CUDA) / Eigen / Ceres / PCL·VTK
# ============================================================================
cmake_minimum_required(VERSION 3.20)

project(three_dimension
        VERSION 0.1.0
        DESCRIPTION "Multi-view 3D reconstruction platform"
        LANGUAGES CXX)

# ---------------------------------------------------------------- 全局设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # 供 clangd / VS Code 用

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ---------------------------------------------------------------- 构建选项
option(RECON_ENABLE_CUDA  "启用 OpenCV CUDA 路径（需显卡与 CUDA Toolkit）" ON)
option(RECON_BUILD_TESTS  "构建单元测试" ON)
option(RECON_BUILD_VIEWER "构建 PCL/VTK 交互查看器" ON)
option(RECON_WARNINGS_AS_ERRORS "把编译告警当作错误" OFF)

if(MSVC)
  # /utf-8 必须有：源码里有中文字符串，不加会变成乱码
  add_compile_options(/utf-8 /W4 /permissive- /Zc:__cplusplus)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)
  if(RECON_WARNINGS_AS_ERRORS)
    add_compile_options(/WX)
  endif()
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
  if(RECON_WARNINGS_AS_ERRORS)
    add_compile_options(-Werror)
  endif()
endif()

# ---------------------------------------------------------------- 依赖查找
# OpenCV：必须是源码编译且打开 WITH_CUDA 的版本，否则 cv::cuda:: 命名空间不可用
find_package(OpenCV 4.5 REQUIRED COMPONENTS
             core imgproc imgcodecs imgproc features2d calib3d)
message(STATUS "OpenCV ${OpenCV_VERSION}: ${OpenCV_DIR}")

if(RECON_ENABLE_CUDA)
  find_package(OpenCV QUIET COMPONENTS cudaarithm cudafeatures2d cudastereo cudawarping)
  if(OpenCV_cudafeatures2d_FOUND)
    set(RECON_HAS_CUDA ON)
    message(STATUS "OpenCV CUDA 模块: 已找到，启用 GPU 加速路径")
  else()
    set(RECON_HAS_CUDA OFF)
    message(WARNING "未找到 OpenCV CUDA 模块，将只编译 CPU 路径。"
                    "确认 OpenCV 是用 -DWITH_CUDA=ON 编译的。")
  endif()
else()
  set(RECON_HAS_CUDA OFF)
  message(STATUS "RECON_ENABLE_CUDA=OFF，只编译 CPU 路径")
endif()

# Eigen：只用 MPL2 许可的几何库
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Ceres：BA 求解器。Windows 下建议用 vcpkg 安装，避免 Suitesparse 编译地狱
find_package(Ceres REQUIRED)
message(STATUS "Ceres ${CERES_VERSION}")

# PCL / VTK：点云与可视化
find_package(PCL 1.10 REQUIRED COMPONENTS common io filters visualization)
message(STATUS "PCL ${PCL_VERSION}")

# 线程与 OpenMP（匹配阶段的并行）
find_package(Threads REQUIRED)
find_package(OpenMP QUIET)

# ---------------------------------------------------------------- 核心库
file(GLOB_RECURSE RECON_SOURCES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM RECON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(recon_core STATIC ${RECON_SOURCES})

target_include_directories(recon_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PCL_INCLUDE_DIRS})

target_link_libraries(recon_core PUBLIC
    ${OpenCV_LIBS}
    Eigen3::Eigen
    Ceres::ceres
    ${PCL_LIBRARIES}
    Threads::Threads)

if(OpenMP_CXX_FOUND)
  target_link_libraries(recon_core PUBLIC OpenMP::OpenMP_CXX)
endif()

target_compile_definitions(recon_core PUBLIC
    $<$<BOOL:${RECON_HAS_CUDA}>:RECON_HAS_CUDA=1>)

# 注意：GLOB 在新增文件时需要重新 configure。
# 若团队觉得别扭，可改为显式列出源文件，或使用 CONFIGURE_DEPENDS（已启用）。

# ---------------------------------------------------------------- 可执行文件
add_executable(three_dimension src/main.cpp)
target_link_libraries(three_dimension PRIVATE recon_core)

# 把配置与脚本拷到输出目录，方便直接双击运行
add_custom_command(TARGET three_dimension POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/configs
            $<TARGET_FILE_DIR:three_dimension>/configs
    COMMENT "复制 configs 到输出目录")

# ---------------------------------------------------------------- 测试
if(RECON_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# ---------------------------------------------------------------- 汇总信息
message(STATUS "-----------------------------------------------------------")
message(STATUS "  OpenCV      : ${OpenCV_VERSION}")
message(STATUS "  CUDA 路径   : ${RECON_HAS_CUDA}")
message(STATUS "  构建类型    : ${CMAKE_BUILD_TYPE}")
message(STATUS "  输出目录    : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "-----------------------------------------------------------")
