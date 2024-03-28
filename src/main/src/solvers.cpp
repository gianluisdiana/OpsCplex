#include <functions.hpp>
#include <ops_bc.hpp>
#include <solvers.hpp>

namespace fs = std::filesystem;

void processModelType(const std::string &model_type) {
  const auto &input_folder = "data/input/" + model_type + "/instances";
  const auto &output_folder = "data/output/" + model_type + "/";
  std::stringstream string_stream;
  for (const auto &file : fs::directory_iterator(input_folder)) {
    std::cout << file.path() << std::endl;
    const auto &instance = createFromFile<emir::OpsInput>(file.path());
    std::ofstream output_os(output_folder + file.path().filename().string());
    output_os << solve<emir::OpsCplexSolver>(instance, 1e-4, string_stream);
    string_stream.str(std::string());
  }
}

void processInputFile(const std::string &inputPath) {
  const auto &instance = createFromFile<emir::OpsInput>(inputPath);
  std::string outputPath = inputPath;
  const std::string instance_folder {"instances/"};
  outputPath.replace(
    outputPath.find(instance_folder), instance_folder.size(), ""
  );
  const std::string input_folder {"input"};
  outputPath.replace(
    outputPath.find(input_folder), input_folder.size(), "output"
  );
  std::ofstream output_os(outputPath);
  std::stringstream string_stream;
  output_os << solve<emir::OpsCplexSolver>(instance, 1e-4, string_stream);
}