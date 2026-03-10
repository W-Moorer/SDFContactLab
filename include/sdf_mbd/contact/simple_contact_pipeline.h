#pragma once

#include <vector>

#include "sdf_mbd/contact/contact_types.h"
#include "sdf_mbd/dynamics/rigid_body.h"

namespace sdf_mbd::contact {

class SimpleContactPipeline {
public:
    explicit SimpleContactPipeline(ContactPipelineOptions options = {}) : options_(options) {}

    [[nodiscard]] std::vector<ContactCluster> compute(const dynamics::RigidBody& body_a,
                                                      const dynamics::RigidBody& body_b) const;

private:
    [[nodiscard]] ContactCandidate make_sphere_sphere_candidate(const dynamics::RigidBody& body_a,
                                                                const dynamics::RigidBody& body_b) const;
    [[nodiscard]] ContactCandidate make_placeholder_candidate(const dynamics::RigidBody& body_a,
                                                              const dynamics::RigidBody& body_b) const;
    [[nodiscard]] ContactCluster to_cluster(const ContactCandidate& candidate) const;

    ContactPipelineOptions options_;
};

}  // namespace sdf_mbd::contact
