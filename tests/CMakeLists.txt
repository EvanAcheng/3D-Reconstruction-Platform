# 单元测试。不引入第三方测试框架，避免给 3 周的项目再加一个依赖；
# 用例失败直接非零退出，由 ctest 捕获。

add_executable(test_geometry test_geometry.cpp)
target_link_libraries(test_geometry PRIVATE recon_core)

add_test(NAME geometry COMMAND test_geometry)

# 后续可加：
# add_executable(test_triangulation test_triangulation.cpp)
# add_executable(test_ba test_ba.cpp)
