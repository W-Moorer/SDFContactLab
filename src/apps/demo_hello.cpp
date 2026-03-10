#include <filesystem>
#include <iostream>

#include "sdf_mbd/contact/simple_contact_pipeline.h"
#include "sdf_mbd/core/config.h"
#include "sdf_mbd/core/version.h"
#include "sdf_mbd/dynamics/rigid_body.h"

int main(int argc, char** argv) {
    const std::filesystem::path case_path =
        argc > 1 ? std::filesystem::path(argv[1])
                 : std::filesystem::path(SDF_CONTACT_LAB_SOURCE_DIR) / "cases" / "sphere_sphere" / "case.json";

    const auto case_config = sdf_mbd::core::load_case_config(case_path);
    const auto bodies = sdf_mbd::dynamics::build_rigid_bodies(case_config);
    const sdf_mbd::contact::SimpleContactPipeline pipeline;
    const auto clusters = pipeline.compute(bodies[0], bodies[1]);

    std::cout << sdf_mbd::core::kProjectName << " " << sdf_mbd::core::kProjectVersion << '\n'
              << "case: " << case_config.name << '\n'
              << "bodies: " << bodies.size() << '\n'
              << "clusters: " << clusters.size() << '\n';

    if (!clusters.empty()) {
        std::cout << "first_gap: " << clusters.front().gn << '\n';
    }

    return 0;
}
