#pragma once

#include <chrono>

namespace sdf_mbd::core {

class ScopedTimer {
public:
    ScopedTimer() : start_(clock::now()) {}

    [[nodiscard]] double elapsed_seconds() const {
        return std::chrono::duration<double>(clock::now() - start_).count();
    }

private:
    using clock = std::chrono::steady_clock;
    clock::time_point start_;
};

}  // namespace sdf_mbd::core
