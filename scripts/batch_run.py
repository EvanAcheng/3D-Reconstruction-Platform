#!/usr/bin/env python3
"""批量实验：遍历多组配置跑同一个数据集，汇总成一张 CSV。

对应需求 FR-M8-03（消融实验）。典型用法是扫描一个参数：

    python scripts/batch_run.py --dataset data/samples/scene01 \
        --sweep match.ransac_threshold_px=1.0,1.5,2.0,3.0

报告里"参数敏感性"那张图的数据就来自这里。
"""

from __future__ import annotations

import argparse
import csv
import itertools
import json
import subprocess
import sys
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="批量运行与汇总")
    parser.add_argument("--dataset", type=Path, required=True)
    parser.add_argument("--exe", type=Path, default=Path("build/bin/three_dimension.exe"))
    parser.add_argument("--config", type=Path, default=Path("configs/default.yaml"))
    parser.add_argument(
        "--sweep",
        action="append",
        default=[],
        help="参数扫描，格式 key=value1,value2；可重复多次做网格",
    )
    parser.add_argument("--out", type=Path, default=Path("results/sweep.csv"))
    return parser.parse_args()


def expand_sweeps(sweeps: list[str]) -> list[dict[str, str]]:
    """把 ['a=1,2', 'b=x,y'] 展开成 4 组参数组合。"""
    keys, values = [], []
    for item in sweeps:
        key, _, raw = item.partition("=")
        keys.append(key.strip())
        values.append([v.strip() for v in raw.split(",")])
    # TODO(FR-M8-03): itertools.product 展开，返回 [{'a': '1', 'b': 'x'}, ...]
    _ = itertools.product
    return [dict(zip(keys, combo)) for combo in itertools.product(*values)] if keys else [{}]


def run_once(args: argparse.Namespace, overrides: dict[str, str], tag: str) -> dict | None:
    """跑一次，返回 metrics.json 内容；失败返回 None。"""
    # TODO(FR-M8-03):
    #   1) 复制默认配置，按 overrides 改字段（key 用点号分隔，如 match.ransac_threshold_px）
    #   2) 写到临时 yaml，调用 subprocess.run([exe, "--dataset", ..., "--config", tmp])
    #   3) 读 results/metrics_*.json，返回其内容
    _ = (args, overrides, tag)
    return None


def main() -> int:
    args = parse_args()
    if not args.exe.exists():
        print(f"找不到可执行文件: {args.exe}（先 cmake --build build）", file=sys.stderr)
        return 1

    combos = expand_sweeps(args.sweep)
    rows = []
    for index, combo in enumerate(combos):
        print(f"[{index + 1}/{len(combos)}] {combo}", file=sys.stderr)
        metrics = run_once(args, combo, tag=str(index))
        if metrics is None:
            print("  运行失败，标记 FAIL 后继续", file=sys.stderr)  # NFR-REL-01：不中断整批
            continue
        rows.append({**combo, **metrics})

    if rows:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        with args.out.open("w", newline="", encoding="utf-8") as handle:
            writer = csv.DictWriter(handle, fieldnames=list(rows[0].keys()))
            writer.writeheader()
            writer.writerows(rows)
        print(f"已写出 {args.out}（{len(rows)} 行）", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
