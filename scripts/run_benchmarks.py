from __future__ import annotations

import argparse
import json
import subprocess
from pathlib import Path


DEFAULT_METHODS = [
    "AnalyticReference",
    "MeshMeshBaseline",
    "SingleSDFAsymmetricBaseline",
    "DirectSDFSingleContactBaseline",
    "OursFullBPC",
]


def resolve_binary(build_dir: Path) -> Path:
    candidates = [
        build_dir / "benchmark_runner.exe",
        build_dir / "Debug" / "benchmark_runner.exe",
        build_dir / "Release" / "benchmark_runner.exe",
        build_dir / "src" / "apps" / "Debug" / "benchmark_runner.exe",
        build_dir / "src" / "apps" / "Release" / "benchmark_runner.exe",
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(f"benchmark_runner not found under {build_dir}")


def main() -> int:
    parser = argparse.ArgumentParser(description="Run all case/method combinations.")
    parser.add_argument("--build-dir", default="build")
    parser.add_argument("--results-dir", default="results/raw")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    cases_dir = root / "cases"
    results_dir = root / args.results_dir
    results_dir.mkdir(parents=True, exist_ok=True)

    binary = resolve_binary(root / args.build_dir)
    summary_csv = root / "results" / "summary.csv"

    for case_path in sorted(cases_dir.glob("*/case.json")):
        case_config = json.loads(case_path.read_text(encoding="utf-8"))
        methods = case_config.get("enabled_methods", DEFAULT_METHODS)
        for method in methods:
            output_path = results_dir / f"{case_path.parent.name}_{method}.json"
            command = [
                str(binary),
                "--case",
                str(case_path),
                "--method",
                method,
                "--output",
                str(output_path),
                "--csv",
                str(summary_csv),
            ]
            print("Running:", " ".join(command))
            completed = subprocess.run(command, check=False)
            if completed.returncode != 0:
                return completed.returncode

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
