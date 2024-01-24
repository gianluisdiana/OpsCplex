#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <OPS_input_t.hpp>
#include <OPS_instance_t.hpp>
#include <OPS_output_t.hpp>
#include <solvers.hpp>

void read(const std::string &file_name, emir::OpsInstance &instance) {
  std::ifstream input_file(file_name);
  input_file >> instance;
  input_file.close();
}

int processor(
  const std::string &ins_file, const std::string &sta_file,
  const std::string &log_file, const int id
) {
  emir::OpsInstance I;
  read(ins_file, I);

  emir::OpsInput In(I);
  const double tol = 1E-4;

  emir::OPS_output_t Out(In);

  std::ofstream O_file(sta_file, std::ios_base::app);
  std::ofstream L_file(log_file);

  (*solver_array[id])(&In, tol, Out, L_file, O_file);

  O_file << '\n';
  O_file.close();

  L_file << '\n';
  L_file.close();

  return 0;
}

int main(int argc, char **argv) {
  int exit_code = 0;

  /*if (argc == 7)
  {*/

  /*
   *  argv[1]       Target file
   *  argv[2]       Instance file
   *  argv[3]       Output log file
   *  argv[4]       Output sol file
   *  argv[5]       Output sta file
   *  argv[6]       Solver ID
   */

  /*const std::string sta_file(argv[1]);
  const std::string ins_file(argv[2]);
  const std::string log_file(argv[3]);

  const int id = atoi(argv[4]) - 1;*/

  const std::string sta_file("tmp.txt");
  const std::string ins_file("/home/riera/Dropbox/Trabajos_en_curso/OPS/"
                             "OPS_CPX/test_ins.txt");
  const std::string log_file("test_log.txt");
  const int id = 0;

  exit_code = processor(ins_file, sta_file, log_file, id);
  /*}*/

  return exit_code;
}
