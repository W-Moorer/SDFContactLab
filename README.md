# SDFContactLab

SDFContactLab is a research-oriented simulation framework skeleton for signed distance field (SDF) based contact modeling in multibody dynamics. The repository is structured to support direct SDF-SDF contact extraction, multi-contact clustering, solver-ready contact Jacobians, and benchmark comparisons against baseline contact methods.

## Status

This repository currently provides:

- a compilable C++20 core library scaffold;
- minimal analytic SDF shapes and a placeholder contact pipeline;
- a benchmark runner, demo executable, and Catch2 tests;
- JSON case files and Python automation scripts for experiments;
- documentation and result export structure aligned with the paper prompt package.

The numerical methods are intentionally minimal in this first skeleton. The interfaces, file layout, and experiment pipeline are designed so the full paper method can be implemented incrementally without reorganizing the repository.

## Repository Layout

```text
.
|-- CMakeLists.txt
|-- docs/
|-- include/sdf_mbd/
|-- src/
|-- tests/
|-- cases/
|-- scripts/
|-- results/
|-- prompt/
`-- local/
```

## Build

Configure and build with CMake:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

Run tests:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

Run the hello demo:

```powershell
.\build\Debug\demo_hello.exe
```

Run a benchmark case:

```powershell
.\build\Debug\benchmark_runner.exe --case cases\sphere_sphere\case.json --method OursFullBPC --output results\sphere_sphere_ours.json
```

Or use the Python wrapper:

```powershell
python scripts\run_case.py --case cases\sphere_sphere\case.json --method OursFullBPC
```

## Design Principles

- New research logic lives in `include/sdf_mbd` and `src`.
- Baselines and paper experiments are first-class parts of the repository.
- Cases are configuration-driven rather than hardcoded in C++.
- Benchmark outputs are machine-readable and suitable for downstream table/figure generation.
- The current contact implementation is a placeholder scaffold, not the full paper method.

## Next Milestones

1. Replace the placeholder contact pipeline with bidirectional projection and clustering.
2. Add voxel SDF support and broad-phase acceleration.
3. Introduce full benchmark statistics, ablations, and paper figure/table generation.
4. Close the dynamics loop with implicit time integration and contact Jacobians.
