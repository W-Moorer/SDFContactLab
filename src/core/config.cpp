#include "sdf_mbd/core/config.h"

#include <fstream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

namespace sdf_mbd::core {

namespace {

Eigen::Vector3d read_vector3(const nlohmann::json& json,
                             const char* key,
                             const Eigen::Vector3d& fallback) {
    if (!json.contains(key)) {
        return fallback;
    }

    const auto& value = json.at(key);
    if (!value.is_array() || value.size() != 3) {
        throw std::runtime_error(std::string("Expected array[3] for key: ") + key);
    }

    return {
        value.at(0).get<double>(),
        value.at(1).get<double>(),
        value.at(2).get<double>()
    };
}

}  // namespace

CaseConfig load_case_config(const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Failed to open case file: " + path.string());
    }

    nlohmann::json json;
    input >> json;

    CaseConfig config;
    config.name = json.value("name", path.stem().string());
    config.time_step = json.value("time_step", config.time_step);
    config.num_steps = json.value("num_steps", config.num_steps);

    if (json.contains("enabled_methods")) {
        config.enabled_methods = json.at("enabled_methods").get<std::vector<std::string>>();
    }

    if (!json.contains("bodies") || !json.at("bodies").is_array()) {
        throw std::runtime_error("Case file must contain a 'bodies' array.");
    }

    for (const auto& body_json : json.at("bodies")) {
        BodyConfig body;
        body.id = body_json.value("id", static_cast<int>(config.bodies.size()));
        body.name = body_json.value("name", "body_" + std::to_string(body.id));
        body.shape = body_json.value("shape", "sphere");
        body.mass = body_json.value("mass", body.mass);
        body.translation = read_vector3(body_json, "translation", body.translation);
        body.rotation_rpy = read_vector3(body_json, "rotation_rpy", body.rotation_rpy);
        body.radius = body_json.value("radius", body.radius);
        body.radii = read_vector3(body_json, "radii", body.radii);
        body.half_extents = read_vector3(body_json, "half_extents", body.half_extents);
        body.normal = read_vector3(body_json, "normal", body.normal);
        body.offset = body_json.value("offset", body.offset);
        config.bodies.push_back(body);
    }

    return config;
}

}  // namespace sdf_mbd::core
