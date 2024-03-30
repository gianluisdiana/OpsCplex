#include <algorithm>
#include <filesystem>

#include <functions.hpp>
#include <main_functions.hpp>
#include <ops_cplex_solver.hpp>

namespace fs = std::filesystem;

input_parser::Parser createParser() {
  return input_parser::Parser()
    .addHelpOption()
    .addOption<input_parser::SingleOption>([] {
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
    .addOption<input_parser::CompoundOption>([] {
      return input_parser::CompoundOption("-m", "--models")
        .addDescription("List of models type to be processed")
        .addConstraint<const std::vector<std::string> &>(
          [](const auto &values) -> bool {
            return std::ranges::all_of(values, [](const auto &value) {
              return value == "A" || value == "B" || value == "C" ||
                     value == "LA" || value == "LB" || value == "LC";
            });
          },
          "The model types must be one of the following: A, B, C, LA, LB, LC"
        )
        .addDefaultValue(std::vector<std::string>());
    });
}

void processInstance(
  const std::string &input_path, const std::string &output_path
) {
  std::ofstream output_os(output_path);
  std::stringstream string_stream;
  output_os << solve<emir::OpsCplexSolver>(input_path, 1e-4, string_stream);
}

void processModelType(const std::string &model_type) {
  const auto &input_folder = "data/input/" + model_type + "/instances";
  const auto &output_folder = "data/output/" + model_type + "/";
  for (const auto &file : fs::directory_iterator(input_folder)) {
    std::cout << file.path() << std::endl;
    processInstance(
      file.path(), output_folder + file.path().filename().string()
    );
  }
}

void processFile(const std::string &input_path) {
  std::string output_path = input_path;
  const std::string instance_folder {"instances/"};
  const std::string input_folder {"input"};
  output_path.replace(
    output_path.find(instance_folder), instance_folder.size(), ""
  );
  output_path.replace(
    output_path.find(input_folder), input_folder.size(), "output"
  );
  processInstance(input_path, output_path);
}