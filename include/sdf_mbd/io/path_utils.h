#pragma once

#include <filesystem>

namespace sdf_mbd::io {

inline std::filesystem::path results_directory() {
    return std::filesystem::path(SDF_CONTACT_LAB_SOURCE_DIR) / "results";
}

}  // namespace sdf_mbd::io
