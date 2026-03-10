#pragma once

#include <string_view>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "sdf_mbd/math/transform.h"

namespace sdf_mbd::geometry {

struct SDFQueryResult {
    double phi{0.0};
    Eigen::Vector3d grad{Eigen::Vector3d::Zero()};
    Eigen::Matrix3d hess{Eigen::Matrix3d::Zero()};
    bool valid{true};
};

struct WorldSDFQueryResult {
    double phi{0.0};
    Eigen::Vector3d grad{Eigen::Vector3d::Zero()};
    Eigen::Matrix3d hess{Eigen::Matrix3d::Zero()};
    bool valid{true};
};

class SDFBase {
public:
    virtual ~SDFBase() = default;

    [[nodiscard]] virtual std::string_view type_name() const noexcept = 0;
    [[nodiscard]] virtual double body_value(const Eigen::Vector3d& body_point) const = 0;
    [[nodiscard]] virtual Eigen::AlignedBox3d body_aabb() const = 0;
    [[nodiscard]] virtual SDFQueryResult query_body(const Eigen::Vector3d& body_point) const;
};

WorldSDFQueryResult query_world(const SDFBase& sdf,
                                const math::Transform3& pose,
                                const Eigen::Vector3d& world_point);

Eigen::Vector3d project_to_surface_world(const SDFBase& sdf,
                                         const math::Transform3& pose,
                                         const Eigen::Vector3d& world_point);

}  // namespace sdf_mbd::geometry
