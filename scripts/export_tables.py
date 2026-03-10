from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description="Export a simple benchmark table from result JSON files.")
    parser.add_argument("--results-dir", default="results/raw")
    parser.add_argument("--output", default="results/table_2_geometry.csv")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    results_dir = root / args.results_dir
    output_path = root / args.output
    output_path.parent.mkdir(parents=True, exist_ok=True)

    rows = []
    for result_file in sorted(results_dir.glob("*.json")):
        payload = json.loads(result_file.read_text(encoding="utf-8"))
        rows.append(
            {
                "method": payload["method"],
                "case": payload["case"],
                "success": payload["success"],
                "gap_error": payload["geometry_metrics"]["gap_error"],
                "cluster_count": payload["geometry_metrics"]["cluster_count"],
                "wall_clock_time": payload["dynamics_metrics"]["wall_clock_time"],
            }
        )

    if not rows:
        raise SystemExit(f"No result JSON files found in {results_dir}")

    with output_path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
