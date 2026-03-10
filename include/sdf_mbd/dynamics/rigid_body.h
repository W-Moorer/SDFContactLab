#pragma once

#include <memory>
#include <vector>

#include "sdf_mbd/core/config.h"
#include "sdf_mbd/geometry/sdf_base.h"
#include "sdf_mbd/math/transform.h"

namespace sdf_mbd::dynamics {

struct RigidBody {
    int id{0};
    std::string name;
    double mass{1.0};
    math::Transform3 pose;
    std::shared_ptr<geometry::SDFBase> sdf;
};

std::vector<RigidBody> build_rigid_bodies(const core::CaseConfig& case_config);

}  // namespace sdf_mbd::dynamics
