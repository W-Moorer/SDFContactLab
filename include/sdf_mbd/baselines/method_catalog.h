#pragma once

#include <array>
#include <string_view>

namespace sdf_mbd::baselines {

inline constexpr std::array<std::string_view, 5> kMethodCatalog = {
    "AnalyticReference",
    "MeshMeshBaseline",
    "SingleSDFAsymmetricBaseline",
    "DirectSDFSingleContactBaseline",
    "OursFullBPC",
};

}  // namespace sdf_mbd::baselines
