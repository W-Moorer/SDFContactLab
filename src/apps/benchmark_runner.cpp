#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

#include "sdf_mbd/contact/simple_contact_pipeline.h"
#include "sdf_mbd/core/config.h"
#include "sdf_mbd/core/timer.h"
#include "sdf_mbd/dynamics/rigid_body.h"
#include "sdf_mbd/eval/metric_calculator.h"
#include "sdf_mbd/eval/result_writer.h"

namespace {

struct CliOptions {
    std::filesystem::path case_path;
    std::string method{"OursFullBPC"};
    std::optional<std::filesystem::path> output_json;
    std::optional<std::filesystem::path> output_csv;
};

void print_usage() {
    std::cerr << "Usage: benchmark_runner --case <path> [--method <name>] [--output <json>] [--csv <csv>]\n";
}

CliOptions parse_args(int argc, char** argv) {
    CliOptions options;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--case" && i + 1 < argc) {
            options.case_path = argv[++i];
        } else if (arg == "--method" && i + 1 < argc) {
            options.method = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            options.output_json = std::filesystem::path(argv[++i]);
        } else if (arg == "--csv" && i + 1 < argc) {
            options.output_csv = std::filesystem::path(argv[++i]);
        } else {
            throw std::runtime_error("Unknown or incomplete argument: " + arg);
        }
    }

    if (options.case_path.empty()) {
        throw std::runtime_error("Missing required --case argument.");
    }

    return options;
}

sdf_mbd::contact::ContactPipelineOptions options_for_method(const std::string& method) {
    sdf_mbd::contact::ContactPipelineOptions options;

    if (method == "DirectSDFSingleContactBaseline") {
        options.num_seeds = 1;
        options.enable_clustering = false;
    } else if (method == "SingleSDFAsymmetricBaseline") {
        options.symmetric_gap = false;
    } else if (method == "MeshMeshBaseline") {
        options.num_seeds = 4;
    }

    return options;
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const CliOptions cli = parse_args(argc, argv);
        const auto case_config = sdf_mbd::core::load_case_config(cli.case_path);
        const auto bodies = sdf_mbd::dynamics::build_rigid_bodies(case_config);

        const auto pipeline_options = options_for_method(cli.method);
        const sdf_mbd::contact::SimpleContactPipeline pipeline(pipeline_options);

        const sdf_mbd::core::ScopedTimer timer;
        const auto clusters = pipeline.compute(bodies[0], bodies[1]);
        const double elapsed_seconds = timer.elapsed_seconds();

        auto record = sdf_mbd::eval::make_benchmark_record(case_config,
                                                           cli.method,
                                                           clusters,
                                                           elapsed_seconds);
        record.reproducibility["case_file"] = cli.case_path.string();
        record.reproducibility["num_seeds"] = pipeline_options.num_seeds;
        record.reproducibility["max_projection_iters"] = pipeline_options.max_projection_iters;
        record.reproducibility["symmetric_gap"] = pipeline_options.symmetric_gap;

        if (cli.output_json.has_value()) {
            sdf_mbd::eval::write_json(record, *cli.output_json);
        }
        if (cli.output_csv.has_value()) {
            sdf_mbd::eval::append_csv(record, *cli.output_csv);
        }

        std::cout << sdf_mbd::eval::to_json(record).dump(2) << '\n';
        return 0;
    } catch (const std::exception& error) {
        print_usage();
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
