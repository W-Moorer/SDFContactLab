#include <filesystem>

#include <Catch2/catch_test_macros.hpp>

#include "sdf_mbd/contact/simple_contact_pipeline.h"
#include "sdf_mbd/core/config.h"
#include "sdf_mbd/dynamics/rigid_body.h"

TEST_CASE("Sphere-sphere case loads and produces one cluster", "[config][contact]") {
    const auto case_path =
        std::filesystem::path(SDF_CONTACT_LAB_SOURCE_DIR) / "cases" / "sphere_sphere" / "case.json";

    const auto case_config = sdf_mbd::core::load_case_config(case_path);
    const auto bodies = sdf_mbd::dynamics::build_rigid_bodies(case_config);
    const sdf_mbd::contact::SimpleContactPipeline pipeline;
    const auto clusters = pipeline.compute(bodies[0], bodies[1]);

    REQUIRE(case_config.bodies.size() == 2);
    REQUIRE(clusters.size() == 1);
    CHECK(std::abs(clusters.front().delta - 0.1) < 1.0e-6);
    CHECK(std::abs(clusters.front().gn + 0.1) < 1.0e-6);
}
