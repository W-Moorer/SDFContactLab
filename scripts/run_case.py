from __future__ import annotations

import argparse
import subprocess
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description="Run a single benchmark case.")
    parser.add_argument("--case", required=True, help="Path to case.json")
    parser.add_argument("--method", default="OursFullBPC")
    parser.add_argument("--build-dir", default="build")
    parser.add_argument("--output", default=None)
    parser.add_argument("--csv", default=None)
    args = parser.parse_args()

    build_dir = Path(args.build_dir)
    candidates = [
        build_dir / "benchmark_runner.exe",
        build_dir / "Debug" / "benchmark_runner.exe",
        build_dir / "Release" / "benchmark_runner.exe",
        build_dir / "src" / "apps" / "Debug" / "benchmark_runner.exe",
        build_dir / "src" / "apps" / "Release" / "benchmark_runner.exe",
    ]
    binary = next((candidate for candidate in candidates if candidate.exists()), None)

    if binary is None:
        raise SystemExit(f"benchmark_runner not found under {build_dir}")

    command = [
        str(binary),
        "--case",
        args.case,
        "--method",
        args.method,
    ]
    if args.output:
        command.extend(["--output", args.output])
    if args.csv:
        command.extend(["--csv", args.csv])

    completed = subprocess.run(command, check=False)
    return completed.returncode


if __name__ == "__main__":
    raise SystemExit(main())
