#pragma once

#include <filesystem>

#include "sdf_mbd/eval/result_types.h"

namespace sdf_mbd::eval {

void write_json(const BenchmarkRecord& record, const std::filesystem::path& path);
void append_csv(const BenchmarkRecord& record, const std::filesystem::path& path);

}  // namespace sdf_mbd::eval
