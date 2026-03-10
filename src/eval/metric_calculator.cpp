#include "sdf_mbd/eval/metric_calculator.h"

#include <cmath>
#include <numeric>

namespace sdf_mbd::eval {

namespace {

std::string fairness_scope_for_method(const std::string& method) {
    if (method == "OursFullBPC" || method == "DirectSDFSingleContactBaseline" ||
        method == "SingleSDFAsymmetricBaseline") {
        return "solver_ready";
    }
    return "geometry_only";
}

}  // namespace

BenchmarkRecord make_benchmark_record(const core::CaseConfig& case_config,
                                      const std::string& method,
                                      const std::vector<contact::ContactCluster>& clusters,
                                      double wall_clock_seconds) {
    BenchmarkRecord record;
    record.method = method;
    record.case_name = case_config.name;
    record.success = !clusters.empty();
    record.fairness_scope = fairness_scope_for_method(method);

    if (!clusters.empty()) {
        const auto& first_cluster = clusters.front();
        const auto candidate_count = std::accumulate(
            clusters.begin(),
            clusters.end(),
            0.0,
            [](double total, const contact::ContactCluster& cluster) {
                return total + static_cast<double>(cluster.members.size());
            });

        record.geometry_metrics.gap_error = std::abs(first_cluster.gn);
        record.geometry_metrics.common_normal_residual =
            first_cluster.members.empty() ? 0.0 : first_cluster.members.front().rn;
        record.geometry_metrics.candidate_count = candidate_count;
        record.geometry_metrics.cluster_count = static_cast<double>(clusters.size());
        record.geometry_metrics.failure_rate = 0.0;
        record.geometry_metrics.cluster_recall = 1.0;
    } else {
        record.geometry_metrics.failure_rate = 1.0;
    }

    record.dynamics_metrics.wall_clock_time = wall_clock_seconds;
    record.dynamics_metrics.mean_newton_iters = record.success ? 1.0 : 0.0;
    record.dynamics_metrics.critical_dt = case_config.time_step;
    record.dynamics_metrics.convergence_radius = record.success ? 1.0 : 0.0;
    record.dynamics_metrics.stiffness_stable_range = record.success ? 1.0 : 0.0;

    record.params = {
        {"time_step", case_config.time_step},
        {"num_steps", case_config.num_steps},
        {"num_bodies", case_config.bodies.size()}
    };

    record.reproducibility = {
        {"experiment_tier", "Tier0Skeleton"},
        {"failure_reason", record.success ? "none" : "all_candidates_filtered"},
        {"dependency_profile", "CMake + Eigen + nlohmann_json + Catch2"}
    };

    return record;
}

}  // namespace sdf_mbd::eval
