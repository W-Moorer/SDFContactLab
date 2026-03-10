#pragma once

#include <vector>

#include <Eigen/Core>

namespace sdf_mbd::contact {

struct ContactCandidate {
    int body_a{-1};
    int body_b{-1};
    Eigen::Vector3d x_a{Eigen::Vector3d::Zero()};
    Eigen::Vector3d x_b{Eigen::Vector3d::Zero()};
    Eigen::Vector3d c{Eigen::Vector3d::Zero()};
    Eigen::Vector3d n{Eigen::Vector3d::UnitY()};
    double gn{0.0};
    double delta{0.0};
    double rx{0.0};
    double rn{0.0};
    bool valid{false};
};

struct ContactCluster {
    int body_a{-1};
    int body_b{-1};
    std::vector<ContactCandidate> members;
    Eigen::Vector3d c{Eigen::Vector3d::Zero()};
    Eigen::Vector3d n{Eigen::Vector3d::UnitY()};
    Eigen::Matrix<double, 3, 2> t{Eigen::Matrix<double, 3, 2>::Zero()};
    double gn{0.0};
    double delta{0.0};
    Eigen::RowVectorXd jn;
    Eigen::MatrixXd jt;
};

struct ContactPipelineOptions {
    int num_seeds{16};
    int max_projection_iters{8};
    double gap_tolerance{1.0e-6};
    bool enable_clustering{true};
    bool symmetric_gap{true};
};

Eigen::Matrix<double, 3, 2> make_tangent_basis(const Eigen::Vector3d& normal);

}  // namespace sdf_mbd::contact
