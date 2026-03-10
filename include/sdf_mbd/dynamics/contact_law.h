#pragma once

#include <algorithm>
#include <cmath>

namespace sdf_mbd::dynamics {

inline double penetration_to_force(double penetration, double stiffness, double alpha) {
    if (penetration <= 0.0) {
        return 0.0;
    }
    return stiffness * std::pow(penetration, alpha);
}

inline double damped_contact_force(double penetration,
                                   double penetration_rate,
                                   double stiffness,
                                   double damping,
                                   double alpha) {
    if (penetration <= 0.0) {
        return 0.0;
    }
    return std::max(0.0, stiffness * std::pow(penetration, alpha) +
                             damping * std::pow(penetration, alpha) * penetration_rate);
}

}  // namespace sdf_mbd::dynamics
