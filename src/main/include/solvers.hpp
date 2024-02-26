#ifndef _SOLVERS_HPP_
#define _SOLVERS_HPP_

#include <OPS_bc1.hpp>
#include <functions.hpp>

template<
  typename T,
  typename std::enable_if<std::is_base_of<emir::OPS_solver_t, T>::value>::type
    * = nullptr>
void solve(
  const emir::OpsInput &input, double tol, std::ostream &os1, std::ostream &os2
) {
  T solver(input, tol);
  solver.solve(os1);
}

int processor(
  const std::string &instance_file_name, const std::string &output_file_name,
  const std::string &log_file_name, const int id
) {
  std::ofstream log_file(log_file_name);
  std::ofstream output_file(output_file_name);
  const double tol = 1e-4;
  const auto input = createFromFile<emir::OpsInput>(instance_file_name);

  solve<emir::OPS_cplex_solver1>(input, tol, log_file, output_file);

  output_file << '\n';
  output_file.close();

  log_file << '\n';
  log_file.close();

  return 0;
}

#endif  // _SOLVERS_HPP_