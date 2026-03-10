#include "sdf_mbd/core/logger.h"

#include <iostream>

namespace sdf_mbd::core {

void log(LogLevel level, std::string_view message) {
    const char* prefix = "[info]";
    if (level == LogLevel::warning) {
        prefix = "[warning]";
    } else if (level == LogLevel::error) {
        prefix = "[error]";
    }

    std::cerr << prefix << ' ' << message << '\n';
}

}  // namespace sdf_mbd::core
