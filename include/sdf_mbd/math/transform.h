#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace sdf_mbd::math {

struct Transform3 {
    Eigen::Matrix3d rotation{Eigen::Matrix3d::Identity()};
    Eigen::Vector3d translation{Eigen::Vector3d::Zero()};

    [[nodiscard]] Eigen::Vector3d to_body(const Eigen::Vector3d& world_point) const {
        return rotation.transpose() * (world_point - translation);
    }

    [[nodiscard]] Eigen::Vector3d to_world(const Eigen::Vector3d& body_point) const {
        return rotation * body_point + translation;
    }

    [[nodiscard]] Eigen::Vector3d rotate_to_world(const Eigen::Vector3d& body_vector) const {
        return rotation * body_vector;
    }

    [[nodiscard]] Eigen::Vector3d rotate_to_body(const Eigen::Vector3d& world_vector) const {
        return rotation.transpose() * world_vector;
    }

    static Transform3 from_rpy_translation(const Eigen::Vector3d& rotation_rpy,
                                           const Eigen::Vector3d& translation) {
        const Eigen::AngleAxisd roll(rotation_rpy.x(), Eigen::Vector3d::UnitX());
        const Eigen::AngleAxisd pitch(rotation_rpy.y(), Eigen::Vector3d::UnitY());
        const Eigen::AngleAxisd yaw(rotation_rpy.z(), Eigen::Vector3d::UnitZ());

        Transform3 transform;
        transform.rotation = (yaw * pitch * roll).toRotationMatrix();
        transform.translation = translation;
        return transform;
    }
};

}  // namespace sdf_mbd::math
