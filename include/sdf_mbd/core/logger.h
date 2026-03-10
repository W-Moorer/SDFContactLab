#pragma once

#include <string_view>

namespace sdf_mbd::core {

enum class LogLevel {
    info,
    warning,
    error
};

void log(LogLevel level, std::string_view message);

}  // namespace sdf_mbd::core
