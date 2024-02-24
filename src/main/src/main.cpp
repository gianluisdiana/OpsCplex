#include <cstdio>
#include <cstdlib>
#include <fstream>

#include <OPS_bc1.hpp>
#include <solvers.hpp>

int processor(
  const std::string &ins_file, const std::string &sta_file,
  const std::string &log_file, const int id
) {
  emir::OpsInput In;
  std::ifstream input_file(ins_file);
  input_file >> In;
  input_file.close();

  const double tol = 1e-4;

  emir::OPS_output_t Out(In);

  std::ofstream O_file(sta_file, std::ios_base::app);
  std::ofstream L_file(log_file);

  solve<emir::OPS_cplex_solver1>(&In, tol, Out, L_file, O_file);

  O_file << '\n';
  O_file.close();

  L_file << '\n';
  L_file.close();

  return 0;
}

int main(int argc, char *argv[]) {
  const std::string sta_file("data/temporal.txt");
  const std::string ins_file("data/test_instance.json");
  const std::string log_file("data/test_log.txt");
  const int id = 0;

  return processor(ins_file, sta_file, log_file, id);
}