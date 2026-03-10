#pragma once

#include <memory>

#include "sdf_mbd/core/config.h"
#include "sdf_mbd/geometry/sdf_base.h"

namespace sdf_mbd::geometry {

class SphereSDF final : public SDFBase {
public:
    explicit SphereSDF(double radius) : radius_(radius) {}

    [[nodiscard]] std::string_view type_name() const noexcept override { return "sphere"; }
    [[nodiscard]] double body_value(const Eigen::Vector3d& body_point) const override;
    [[nodiscard]] SDFQueryResult query_body(const Eigen::Vector3d& body_point) const override;
    [[nodiscard]] Eigen::AlignedBox3d body_aabb() const override;
    [[nodiscard]] double radius() const noexcept { return radius_; }

private:
    double radius_{1.0};
};

class EllipsoidSDF final : public SDFBase {
public:
    explicit EllipsoidSDF(Eigen::Vector3d radii) : radii_(std::move(radii)) {}

    [[nodiscard]] std::string_view type_name() const noexcept override { return "ellipsoid"; }
    [[nodiscard]] double body_value(const Eigen::Vector3d& body_point) const override;
    [[nodiscard]] Eigen::AlignedBox3d body_aabb() const override;

private:
    Eigen::Vector3d radii_{Eigen::Vector3d::Ones()};
};

class BoxSDF final : public SDFBase {
public:
    explicit BoxSDF(Eigen::Vector3d half_extents) : half_extents_(std::move(half_extents)) {}

    [[nodiscard]] std::string_view type_name() const noexcept override { return "box"; }
    [[nodiscard]] double body_value(const Eigen::Vector3d& body_point) const override;
    [[nodiscard]] Eigen::AlignedBox3d body_aabb() const override;

private:
    Eigen::Vector3d half_extents_{Eigen::Vector3d::Constant(0.5)};
};

class PlaneSDF final : public SDFBase {
public:
    PlaneSDF(Eigen::Vector3d normal, double offset);

    [[nodiscard]] std::string_view type_name() const noexcept override { return "plane"; }
    [[nodiscard]] double body_value(const Eigen::Vector3d& body_point) const override;
    [[nodiscard]] SDFQueryResult query_body(const Eigen::Vector3d& body_point) const override;
    [[nodiscard]] Eigen::AlignedBox3d body_aabb() const override;

private:
    Eigen::Vector3d normal_{Eigen::Vector3d::UnitY()};
    double offset_{0.0};
};

std::shared_ptr<SDFBase> make_analytic_sdf(const core::BodyConfig& body_config);

}  // namespace sdf_mbd::geometry
