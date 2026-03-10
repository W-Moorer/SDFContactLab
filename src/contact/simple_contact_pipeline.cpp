#include "sdf_mbd/contact/simple_contact_pipeline.h"

#include <cmath>
#include <stdexcept>

#include "sdf_mbd/geometry/analytic_sdfs.h"

namespace sdf_mbd::contact {

Eigen::Matrix<double, 3, 2> make_tangent_basis(const Eigen::Vector3d& normal) {
    Eigen::Vector3d n = normal;
    if (n.norm() < 1.0e-12) {
        n = Eigen::Vector3d::UnitY();
    } else {
        n.normalize();
    }

    const Eigen::Vector3d seed = std::abs(n.dot(Eigen::Vector3d::UnitX())) < 0.9
                                     ? Eigen::Vector3d::UnitX()
                                     : Eigen::Vector3d::UnitY();
    Eigen::Vector3d t1 = (seed - seed.dot(n) * n).normalized();
    Eigen::Vector3d t2 = n.cross(t1).normalized();

    Eigen::Matrix<double, 3, 2> basis;
    basis.col(0) = t1;
    basis.col(1) = t2;
    return basis;
}

std::vector<ContactCluster> SimpleContactPipeline::compute(const dynamics::RigidBody& body_a,
                                                           const dynamics::RigidBody& body_b) const {
    ContactCandidate candidate;

    if (dynamic_cast<const geometry::SphereSDF*>(body_a.sdf.get()) != nullptr &&
        dynamic_cast<const geometry::SphereSDF*>(body_b.sdf.get()) != nullptr) {
        candidate = make_sphere_sphere_candidate(body_a, body_b);
    } else {
        candidate = make_placeholder_candidate(body_a, body_b);
    }

    if (!candidate.valid) {
        return {};
    }

    return {to_cluster(candidate)};
}

ContactCandidate SimpleContactPipeline::make_sphere_sphere_candidate(const dynamics::RigidBody& body_a,
                                                                     const dynamics::RigidBody& body_b) const {
    const auto* sphere_a = dynamic_cast<const geometry::SphereSDF*>(body_a.sdf.get());
    const auto* sphere_b = dynamic_cast<const geometry::SphereSDF*>(body_b.sdf.get());
    if (sphere_a == nullptr || sphere_b == nullptr) {
        throw std::runtime_error("Sphere candidate requested for non-sphere bodies.");
    }

    ContactCandidate candidate;
    candidate.body_a = body_a.id;
    candidate.body_b = body_b.id;

    const Eigen::Vector3d center_a = body_a.pose.translation;
    const Eigen::Vector3d center_b = body_b.pose.translation;
    Eigen::Vector3d normal = center_b - center_a;
    const double distance = normal.norm();
    if (distance > 1.0e-12) {
        normal /= distance;
    } else {
        normal = Eigen::Vector3d::UnitX();
    }

    candidate.x_a = center_a + sphere_a->radius() * normal;
    candidate.x_b = center_b - sphere_b->radius() * normal;
    candidate.c = 0.5 * (candidate.x_a + candidate.x_b);
    candidate.n = normal;
    candidate.gn = distance - (sphere_a->radius() + sphere_b->radius());
    candidate.delta = std::max(0.0, -candidate.gn);
    candidate.rx = 0.0;
    candidate.rn = 0.0;
    candidate.valid = true;
    return candidate;
}

ContactCandidate SimpleContactPipeline::make_placeholder_candidate(const dynamics::RigidBody& body_a,
                                                                   const dynamics::RigidBody& body_b) const {
    ContactCandidate candidate;
    candidate.body_a = body_a.id;
    candidate.body_b = body_b.id;

    Eigen::Vector3d x_a = body_a.pose.translation;
    Eigen::Vector3d x_b = body_b.pose.translation;
    Eigen::Vector3d previous = x_a;

    for (int iteration = 0; iteration < options_.max_projection_iters; ++iteration) {
        x_b = geometry::project_to_surface_world(*body_b.sdf, body_b.pose, x_a);
        x_a = geometry::project_to_surface_world(*body_a.sdf, body_a.pose, x_b);
        if ((x_a - previous).norm() <= options_.gap_tolerance) {
            break;
        }
        previous = x_a;
    }

    const auto query_a = geometry::query_world(*body_a.sdf, body_a.pose, x_a);
    const auto query_b = geometry::query_world(*body_b.sdf, body_b.pose, x_b);

    Eigen::Vector3d n_a = Eigen::Vector3d::UnitY();
    if (query_a.grad.norm() > 1.0e-12) {
        n_a = query_a.grad.normalized();
    }

    Eigen::Vector3d n_b = -n_a;
    if (query_b.grad.norm() > 1.0e-12) {
        n_b = query_b.grad.normalized();
    }

    candidate.x_a = x_a;
    candidate.x_b = x_b;
    candidate.c = 0.5 * (x_a + x_b);
    candidate.n = (n_a - n_b).norm() > 1.0e-12 ? (n_a - n_b).normalized() : n_a;
    candidate.gn = options_.symmetric_gap ? 0.5 * (query_b.phi + query_a.phi) : query_b.phi;
    candidate.delta = std::max(0.0, -candidate.gn);
    candidate.rx = (x_a - previous).norm();
    candidate.rn = 1.0 + n_a.dot(n_b);
    candidate.valid = query_a.valid && query_b.valid;
    return candidate;
}

ContactCluster SimpleContactPipeline::to_cluster(const ContactCandidate& candidate) const {
    ContactCluster cluster;
    cluster.body_a = candidate.body_a;
    cluster.body_b = candidate.body_b;
    cluster.members.push_back(candidate);
    cluster.c = candidate.c;
    cluster.n = candidate.n;
    cluster.t = make_tangent_basis(candidate.n);
    cluster.gn = candidate.gn;
    cluster.delta = candidate.delta;
    cluster.jn = Eigen::RowVectorXd::Zero(12);
    cluster.jt = Eigen::MatrixXd::Zero(2, 12);
    return cluster;
}

}  // namespace sdf_mbd::contact
