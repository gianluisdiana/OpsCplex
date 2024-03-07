#include <solvers.hpp>

/**
 * @brief Main function to process the instance of the problem and generates
 * the solution with the output and the log files.
 */
int main(int argc, char *argv[]) {
  const auto output_file_name = "data/test_output.json";
  const auto instance_file_name = "data/input/A/instances/A_n040_001_a50_002.txt";
  const auto log_file_name = "data/test_log.txt";
  const int id = 0;

  return processor(instance_file_name, output_file_name, log_file_name, id);
}