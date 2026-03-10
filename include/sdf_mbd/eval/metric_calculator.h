#pragma once

#include <string>
#include <vector>

#include "sdf_mbd/contact/contact_types.h"
#include "sdf_mbd/core/config.h"
#include "sdf_mbd/eval/result_types.h"

namespace sdf_mbd::eval {

BenchmarkRecord make_benchmark_record(const core::CaseConfig& case_config,
                                      const std::string& method,
                                      const std::vector<contact::ContactCluster>& clusters,
                                      double wall_clock_seconds);

}  // namespace sdf_mbd::eval
