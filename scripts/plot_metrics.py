#!/usr/bin/env python3
"""把 C++ 输出的 metrics.json / ba_log.csv 画成报告用的图表。

对应需求 FR-M8-02。生成的图会被直接贴进阶段报告：
    fig_reprojection_hist.png  重投影误差分布
    fig_ba_convergence.png     BA 代价下降曲线
    fig_stage_timing.png       各阶段耗时
    fig_ablation.png           参数敏感性（需要多组运行结果）

用法:
    python scripts/plot_metrics.py results/
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="指标绘图")
    parser.add_argument("results_dir", type=Path, help="包含 metrics_*.json 的目录")
    parser.add_argument("--output-dir", type=Path, default=Path("results/figures"))
    return parser.parse_args()


def load_metrics(results_dir: Path) -> list[dict]:
    """读出目录下所有 metrics_*.json。"""
    # TODO(FR-M8-02): sorted(results_dir.glob("metrics_*.json")) -> json.load
    return []


def plot_reprojection(errors: list[float], output: Path) -> None:
    # TODO(FR-M8-02): matplotlib 直方图，标出 mean 与 p95 两条竖线
    pass


def plot_ba_convergence(csv_path: Path, output: Path) -> None:
    # TODO(FR-M8-02): pandas.read_csv -> 画 initial/final cost 随迭代下降
    pass


def plot_stage_timing(metrics: dict, output: Path) -> None:
    # TODO(FR-M8-02): 横向条形图，按耗时排序，一眼看出瓶颈阶段
    pass


def main() -> int:
    args = parse_args()
    if not args.results_dir.is_dir():
        print(f"目录不存在: {args.results_dir}", file=sys.stderr)
        return 1
    args.output_dir.mkdir(parents=True, exist_ok=True)

    metrics = load_metrics(args.results_dir)
    print(f"找到 {len(metrics)} 组结果（TODO: 实现绘图）", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
