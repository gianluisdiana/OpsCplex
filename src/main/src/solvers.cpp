#include <ops_bc.hpp>
#include <solvers.hpp>

namespace fs = std::filesystem;

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