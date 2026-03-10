#pragma once

namespace sdf_mbd::solver {

struct SolverStepReport {
    int newton_iterations{0};
    double residual_norm{0.0};
    bool converged{false};
};

}  // namespace sdf_mbd::solver
