#include <functions.hpp>
#include <ops_bc.hpp>
#include <solvers.hpp>

int processor(
  const std::string &instance_file_name, const std::string &output_file_name,
  const std::string &log_file_name, const int id
) {
  std::ofstream log_file(log_file_name);
  std::ofstream output_file(output_file_name);
  const double tolerance = 1e-4;
  const auto input = createFromFile<emir::OpsInput>(instance_file_name);

  output_file << solve<emir::OpsCplexSolver>(input, tolerance, log_file);

  output_file.close();
  log_file.close();

  return 0;
}