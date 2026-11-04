#!/usr/bin/env python3
"""用 Open3D 快速查看点云 —— 开发期的主要调试手段，也是演示的兜底方案。

为什么需要它：PCL/VTK 程序编译慢、崩溃后不容易定位，
先用 Open3D 确认 PLY 本身是对的，再回头查 C++ 侧的问题。

用法:
    python scripts/view_pointcloud_open3d.py results/dense.ply
    python scripts/view_pointcloud_open3d.py results/dense.ply --screenshot out.png
"""

from __future__ import annotations

import argparse
import sys


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Open3D 点云查看器")
    parser.add_argument("path", help="PLY / PCD / XYZ 文件路径")
    parser.add_argument("--screenshot", default=None, help="截图保存路径")
    parser.add_argument("--point-size", type=float, default=2.0)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        import open3d as o3d  # noqa: PLC0415  (延迟导入，避免没有 GPU 环境时启动就报错)
    except ImportError:
        print("未安装 open3d：pip install open3d", file=sys.stderr)
        return 1

    # TODO(FR-M7-07): 读取点云并显示
    #   pcd = o3d.io.read_point_cloud(args.path)
    #   print(f"点数: {len(pcd.points)}, 包围盒: {pcd.get_axis_aligned_bounding_box()}")
    #   o3d.visualization.draw_geometries([pcd], window_name="Point Cloud")
    #   if args.screenshot: 用 OffscreenRenderer 渲染并保存 PNG
    print(f"[TODO] 待实现：{args.path}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
