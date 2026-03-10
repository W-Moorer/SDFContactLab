# Architecture

The repository is organized around the separation between geometry, contact extraction, dynamics coupling, baseline methods, and evaluation:

- `core`: configuration loading, timing, logging, and versioning;
- `math`: rigid transforms and low-level geometric utilities;
- `geometry`: analytic and future voxel/neural SDF representations;
- `contact`: contact candidates, clusters, and extraction pipelines;
- `dynamics`: rigid bodies, contact laws, and solver-ready data;
- `solver`: placeholders for implicit and nonsmooth time integration;
- `baselines`: category-faithful baseline methods;
- `eval`: metrics, result export, and benchmark aggregation.

The current skeleton intentionally implements only a minimal placeholder contact pipeline so the repository remains compilable while larger research components are introduced phase by phase.
