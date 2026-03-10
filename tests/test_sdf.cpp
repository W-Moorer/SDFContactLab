#include <cmath>

#include <Catch2/catch_test_macros.hpp>

#include "sdf_mbd/geometry/analytic_sdfs.h"

TEST_CASE("Sphere SDF returns zero on the surface", "[geometry]") {
    const sdf_mbd::geometry::SphereSDF sphere(2.0);
    const auto query = sphere.query_body(Eigen::Vector3d(2.0, 0.0, 0.0));

    REQUIRE(query.valid);
    CHECK(std::abs(query.phi - 0.0) < 1.0e-12);
    CHECK(std::abs(query.grad.x() - 1.0) < 1.0e-12);
}

TEST_CASE("Plane SDF produces constant gradient", "[geometry]") {
    const sdf_mbd::geometry::PlaneSDF plane(Eigen::Vector3d::UnitY(), 0.0);
    const auto query = plane.query_body(Eigen::Vector3d(0.0, 1.25, 0.0));

    REQUIRE(query.valid);
    CHECK(std::abs(query.phi - 1.25) < 1.0e-12);
    CHECK(std::abs(query.grad.y() - 1.0) < 1.0e-12);
}
