#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <Eigen/Core>

namespace sdf_mbd::core {

struct BodyConfig {
    int id{0};
    std::string name;
    std::string shape;
    double mass{1.0};
    Eigen::Vector3d translation{Eigen::Vector3d::Zero()};
    Eigen::Vector3d rotation_rpy{Eigen::Vector3d::Zero()};
    double radius{1.0};
    Eigen::Vector3d radii{Eigen::Vector3d::Ones()};
    Eigen::Vector3d half_extents{Eigen::Vector3d::Constant(0.5)};
    Eigen::Vector3d normal{Eigen::Vector3d::UnitY()};
    double offset{0.0};
};

struct CaseConfig {
    std::string name;
    double time_step{1.0e-3};
    int num_steps{1};
    std::vector<std::string> enabled_methods;
    std::vector<BodyConfig> bodies;
};

CaseConfig load_case_config(const std::filesystem::path& path);

}  // namespace sdf_mbd::core
