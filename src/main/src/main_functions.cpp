#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <input_parser/parser.hpp>

#include <functions.hpp>
#include <main_functions.hpp>
#include <ops_cplex_solver.hpp>

namespace fs = std::filesystem;

struct PathConfig {
  std::string input_path;
  std::string output_path;
};

input_parser::Parser createParser() {
  return input_parser::Parser()
    .addHelpOption()
    .addOption([] {
      return input_parser::SingleOption("-i", "--input")
        .addDescription("Path to the input file to be processed")
        .addDefaultValue(std::string())
        .addConstraint<const std::string &>(
          [](const auto &value) -> bool {
            return value.empty() ||
                   (!value.empty() && std::filesystem::exists(value));
          },
          "The file must exist!"
        );
    })
    .addOption([] {
      return input_parser::CompoundOption("-m", "--models")
        .addDescription("List of models type to be processed")
        .addConstraint<std::vector<std::string>>(
          [](const auto &values) -> bool {
            return std::ranges::all_of(values, [](const auto &value) {
              return value == "A" || value == "B" || value == "C" ||
                     value == "LA" || value == "LB" || value == "LC";
            });
          },
          "The model types must be one of the following: A, B, C, LA, LB, LC"
        )
        .addDefaultValue(std::vector<std::string>());
    })
    .addOption([] {
      return input_parser::SingleOption("-t", "--tolerance")
        .addDescription("Tolerance for the solver")
        .addDefaultValue(1e-4)  // NOLINT
        .addConstraint<double>(
          [](const auto &value) -> bool { return value > 0; },
          "The tolerance must be greater than 0"
        );
    });
}

void processInstance(const PathConfig &path_config, const double tolerance) {
  std::ofstream output_os(path_config.output_path);
  std::stringstream string_stream;
  output_os << solve<emir::OpsCplexSolver>(
    path_config.input_path, tolerance, string_stream
  );
  // NOTE: SEGFAULT TRYING TO ASSIGN TO VARIABLE AND PRINT IT
}

void processModelType(const std::string &model_type, const double tolerance) {
  const auto &input_folder = "data/input/" + model_type + "/instances";
  const auto &output_folder = "data/output/" + model_type + "/";
  for (const auto &file : fs::directory_iterator(input_folder)) {
    std::cout << file.path() << '\n';
    processInstance(
      PathConfig {
        .input_path = file.path(),
        .output_path = output_folder + file.path().filename().string()
      },
      tolerance
    );
  }
}

void processFile(const std::string &input_path, const double tolerance) {
  std::string output_path = input_path;
  const std::string instance_folder {"instances/"};
  const std::string input_folder {"input"};
  output_path.replace(
    output_path.find(instance_folder), instance_folder.size(), ""
  );
  output_path.replace(
    output_path.find(input_folder), input_folder.size(), "output"
  );
  processInstance(
    PathConfig {.input_path = input_path, .output_path = output_path}, tolerance
  );
}