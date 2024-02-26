#include <solvers.hpp>

int main(int argc, char *argv[]) {
  const auto output_file_name = "data/temporal.txt";
  const auto instance_file_name = "data/test_instance.json";
  const auto log_file_name = "data/test_log.txt";
  const int id = 0;

  return processor(instance_file_name, output_file_name, log_file_name, id);
}