#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace sdf_mbd::eval {

struct GeometryMetrics {
    double gap_error{0.0};
    double normal_angle_error_deg{0.0};
    double center_error{0.0};
    double common_normal_residual{0.0};
    double candidate_count{0.0};
    double cluster_count{0.0};
    double failure_rate{0.0};
    double cluster_recall{0.0};
    double normal_jitter{0.0};
};

struct DynamicsMetrics {
    double energy_drift{0.0};
    double momentum_change{0.0};
    double angular_momentum_change{0.0};
    double normal_impulse_error{0.0};
    double constraint_residual{0.0};
    double mean_newton_iters{0.0};
    double solver_fail_rate{0.0};
    double critical_dt{0.0};
    double convergence_radius{0.0};
    double stiffness_stable_range{0.0};
    double wall_clock_time{0.0};
};

struct BenchmarkRecord {
    std::string method;
    std::string case_name;
    int seed{0};
    bool success{false};
    std::string fairness_scope{"geometry_only"};
    GeometryMetrics geometry_metrics;
    DynamicsMetrics dynamics_metrics;
    nlohmann::json params;
    nlohmann::json reproducibility;
};

nlohmann::json to_json(const BenchmarkRecord& record);
std::string csv_header();
std::string csv_row(const BenchmarkRecord& record);

}  // namespace sdf_mbd::eval
