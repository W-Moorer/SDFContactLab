#include "sdf_mbd/geometry/analytic_sdfs.h"

#include <cmath>
#include <stdexcept>

namespace sdf_mbd::geometry {

SDFQueryResult SDFBase::query_body(const Eigen::Vector3d& body_point) const {
    constexpr double h = 1.0e-6;
    SDFQueryResult result;
    result.phi = body_value(body_point);

    for (int axis = 0; axis < 3; ++axis) {
        Eigen::Vector3d plus = body_point;
        Eigen::Vector3d minus = body_point;
        plus(axis) += h;
        minus(axis) -= h;
        result.grad(axis) = (body_value(plus) - body_value(minus)) / (2.0 * h);
    }

    result.valid = std::isfinite(result.phi) && result.grad.allFinite();
    return result;
}

WorldSDFQueryResult query_world(const SDFBase& sdf,
                                const math::Transform3& pose,
                                const Eigen::Vector3d& world_point) {
    const Eigen::Vector3d body_point = pose.to_body(world_point);
    const SDFQueryResult body_result = sdf.query_body(body_point);

    WorldSDFQueryResult world_result;
    world_result.phi = body_result.phi;
    world_result.grad = pose.rotate_to_world(body_result.grad);
    world_result.hess = pose.rotation * body_result.hess * pose.rotation.transpose();
    world_result.valid = body_result.valid;
    return world_result;
}

Eigen::Vector3d project_to_surface_world(const SDFBase& sdf,
                                         const math::Transform3& pose,
                                         const Eigen::Vector3d& world_point) {
    const WorldSDFQueryResult query = query_world(sdf, pose, world_point);
    const double grad_norm = query.grad.norm();
    if (grad_norm < 1.0e-12) {
        return world_point;
    }

    return world_point - query.phi * (query.grad / grad_norm);
}

double SphereSDF::body_value(const Eigen::Vector3d& body_point) const {
    return body_point.norm() - radius_;
}

SDFQueryResult SphereSDF::query_body(const Eigen::Vector3d& body_point) const {
    SDFQueryResult result;
    const double distance = body_point.norm();
    result.phi = distance - radius_;
    if (distance > 1.0e-12) {
        result.grad = body_point / distance;
    } else {
        result.grad = Eigen::Vector3d::UnitX();
    }
    result.valid = true;
    return result;
}

Eigen::AlignedBox3d SphereSDF::body_aabb() const {
    return {Eigen::Vector3d::Constant(-radius_), Eigen::Vector3d::Constant(radius_)};
}

double EllipsoidSDF::body_value(const Eigen::Vector3d& body_point) const {
    const Eigen::Array3d scaled = body_point.array() / radii_.array();
    const double k = scaled.matrix().norm();
    return (k - 1.0) * radii_.minCoeff();
}

Eigen::AlignedBox3d EllipsoidSDF::body_aabb() const {
    return {-radii_, radii_};
}

double BoxSDF::body_value(const Eigen::Vector3d& body_point) const {
    const Eigen::Vector3d q = body_point.cwiseAbs() - half_extents_;
    const Eigen::Vector3d outside = q.cwiseMax(0.0);
    const double outside_distance = outside.norm();
    const double inside_distance = std::min(0.0, q.maxCoeff());
    return outside_distance + inside_distance;
}

Eigen::AlignedBox3d BoxSDF::body_aabb() const {
    return {-half_extents_, half_extents_};
}

PlaneSDF::PlaneSDF(Eigen::Vector3d normal, double offset) : offset_(offset) {
    const double norm = normal.norm();
    if (norm > 1.0e-12) {
        normal_ = normal / norm;
    } else {
        normal_ = Eigen::Vector3d::UnitY();
    }
}

double PlaneSDF::body_value(const Eigen::Vector3d& body_point) const {
    return normal_.dot(body_point) - offset_;
}

SDFQueryResult PlaneSDF::query_body(const Eigen::Vector3d& body_point) const {
    SDFQueryResult result;
    result.phi = body_value(body_point);
    result.grad = normal_;
    result.valid = true;
    return result;
}

Eigen::AlignedBox3d PlaneSDF::body_aabb() const {
    const Eigen::Vector3d lo = Eigen::Vector3d::Constant(-1.0e6);
    const Eigen::Vector3d hi = Eigen::Vector3d::Constant(1.0e6);
    return {lo, hi};
}

std::shared_ptr<SDFBase> make_analytic_sdf(const core::BodyConfig& body_config) {
    if (body_config.shape == "sphere") {
        return std::make_shared<SphereSDF>(body_config.radius);
    }
    if (body_config.shape == "ellipsoid") {
        return std::make_shared<EllipsoidSDF>(body_config.radii);
    }
    if (body_config.shape == "box") {
        return std::make_shared<BoxSDF>(body_config.half_extents);
    }
    if (body_config.shape == "plane") {
        return std::make_shared<PlaneSDF>(body_config.normal, body_config.offset);
    }

    throw std::runtime_error("Unsupported analytic SDF shape: " + body_config.shape);
}

}  // namespace sdf_mbd::geometry
