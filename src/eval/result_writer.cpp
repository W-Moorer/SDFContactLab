#include "sdf_mbd/eval/result_writer.h"

#include <fstream>
#include <sstream>

namespace sdf_mbd::eval {

nlohmann::json to_json(const BenchmarkRecord& record) {
    return {
        {"method", record.method},
        {"case", record.case_name},
        {"seed", record.seed},
        {"success", record.success},
        {"fairness_scope", record.fairness_scope},
        {"geometry_metrics",
         {
             {"gap_error", record.geometry_metrics.gap_error},
             {"normal_angle_error_deg", record.geometry_metrics.normal_angle_error_deg},
             {"center_error", record.geometry_metrics.center_error},
             {"common_normal_residual", record.geometry_metrics.common_normal_residual},
             {"candidate_count", record.geometry_metrics.candidate_count},
             {"cluster_count", record.geometry_metrics.cluster_count},
             {"failure_rate", record.geometry_metrics.failure_rate},
             {"cluster_recall", record.geometry_metrics.cluster_recall},
             {"normal_jitter", record.geometry_metrics.normal_jitter}
         }},
        {"dynamics_metrics",
         {
             {"energy_drift", record.dynamics_metrics.energy_drift},
             {"momentum_change", record.dynamics_metrics.momentum_change},
             {"angular_momentum_change", record.dynamics_metrics.angular_momentum_change},
             {"normal_impulse_error", record.dynamics_metrics.normal_impulse_error},
             {"constraint_residual", record.dynamics_metrics.constraint_residual},
             {"mean_newton_iters", record.dynamics_metrics.mean_newton_iters},
             {"solver_fail_rate", record.dynamics_metrics.solver_fail_rate},
             {"critical_dt", record.dynamics_metrics.critical_dt},
             {"convergence_radius", record.dynamics_metrics.convergence_radius},
             {"stiffness_stable_range", record.dynamics_metrics.stiffness_stable_range},
             {"wall_clock_time", record.dynamics_metrics.wall_clock_time}
         }},
        {"params", record.params},
        {"reproducibility", record.reproducibility}
    };
}

std::string csv_header() {
    return "method,case,success,fairness_scope,gap_error,cluster_count,wall_clock_time\n";
}

std::string csv_row(const BenchmarkRecord& record) {
    std::ostringstream stream;
    stream << record.method << ','
           << record.case_name << ','
           << (record.success ? 1 : 0) << ','
           << record.fairness_scope << ','
           << record.geometry_metrics.gap_error << ','
           << record.geometry_metrics.cluster_count << ','
           << record.dynamics_metrics.wall_clock_time << '\n';
    return stream.str();
}

void write_json(const BenchmarkRecord& record, const std::filesystem::path& path) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream output(path);
    output << to_json(record).dump(2) << '\n';
}

void append_csv(const BenchmarkRecord& record, const std::filesystem::path& path) {
    const bool write_header = !std::filesystem::exists(path);
    std::filesystem::create_directories(path.parent_path());

    std::ofstream output(path, std::ios::app);
    if (write_header) {
        output << csv_header();
    }
    output << csv_row(record);
}

}  // namespace sdf_mbd::eval
