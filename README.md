# 多视觉融合的三维重建平台

C++17 实现的增量式 SfM + 稠密重建原型，配套 Python 辅助脚本。

- 需求文档：[docs/需求分析.md](docs/需求分析.md)
- 详细设计见 `docs/`（待补）

## 目录

```
include/     C++ 头文件（对外接口）
src/         C++ 实现（当前为骨架，函数体均为 TODO）
apps/        暂无（入口在 src/main.cpp）
scripts/     Python 辅助脚本
configs/     YAML 配置
data/        数据集与标定文件
tests/       单元测试
build/       构建目录（不入库）
```

## 构建

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -j
```

前提：OpenCV 已源码编译并开启 `WITH_CUDA`，Eigen / Ceres / PCL 已安装。
具体版本与路径见 `docs/env.md`（待补）。

## 运行

```powershell
build/bin/three_dimension.exe --dataset data/samples/scene01 --config configs/default.yaml
```

## 当前状态

所有 `.cpp` 的函数体只包含 `// TODO(需求编号)` 注释，用于标出需要调用的
OpenCV / Ceres / PCL 接口。实现顺序建议按需求文档的 P0 清单：
`M1 环境 → M2 特征 → M3 匹配 → M4 SfM → M5 BA → M6 MVS → M7 可视化`。
