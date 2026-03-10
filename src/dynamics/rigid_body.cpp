#include "sdf_mbd/dynamics/rigid_body.h"

#include <stdexcept>
#include <utility>

#include "sdf_mbd/geometry/analytic_sdfs.h"

namespace sdf_mbd::dynamics {

std::vector<RigidBody> build_rigid_bodies(const core::CaseConfig& case_config) {
    std::vector<RigidBody> bodies;
    bodies.reserve(case_config.bodies.size());

    for (const auto& body_config : case_config.bodies) {
        RigidBody body;
        body.id = body_config.id;
        body.name = body_config.name;
        body.mass = body_config.mass;
        body.pose = math::Transform3::from_rpy_translation(body_config.rotation_rpy,
                                                           body_config.translation);
        body.sdf = geometry::make_analytic_sdf(body_config);
        bodies.push_back(std::move(body));
    }

    if (bodies.size() < 2) {
        throw std::runtime_error("A benchmark case must define at least two bodies.");
    }

    return bodies;
}

}  // namespace sdf_mbd::dynamics
