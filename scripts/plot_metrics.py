from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description="Plot simple wall clock metrics from JSON results.")
    parser.add_argument("--results-dir", default="results/raw")
    parser.add_argument("--output", default="results/wall_clock_time.png")
    args = parser.parse_args()

    try:
        import matplotlib.pyplot as plt
    except ModuleNotFoundError:
        raise SystemExit("matplotlib is required for plotting")

    root = Path(__file__).resolve().parents[1]
    results_dir = root / args.results_dir
    data = []

    for result_file in sorted(results_dir.glob("*.json")):
        payload = json.loads(result_file.read_text(encoding="utf-8"))
        data.append((payload["method"], payload["case"], payload["dynamics_metrics"]["wall_clock_time"]))

    if not data:
        raise SystemExit(f"No result JSON files found in {results_dir}")

    labels = [f"{case}\n{method}" for method, case, _ in data]
    values = [value for _, _, value in data]

    fig, ax = plt.subplots(figsize=(12, 5))
    ax.bar(labels, values)
    ax.set_ylabel("Wall clock time [s]")
    ax.set_title("Benchmark runner wall clock time")
    ax.tick_params(axis="x", rotation=45)
    fig.tight_layout()
    fig.savefig(root / args.output, dpi=150)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
