#ifndef _SOLVERS_HPP_
#define _SOLVERS_HPP_

#include <functions.hpp>
#include <ops_bc.hpp>

/**
 * @brief Solve the O.P.S. problem using the given solver.
 * Also output the log and the solution in the given outflows.
 *
 * @tparam T The type of the solver to be used. It can be any class that
 * inherits from emir::OpsSolver.
 * @param input The input of the solver with the instance data.
 * @param tolerance The maximum tolerance to be used in the solver.
 * @param log_os The outflow where the logs will be stored.
 * @param solution_os The outflow where the solution will be stored.
 */
template<
  typename T, typename std::enable_if<
                std::is_base_of<emir::OpsSolver, T>::value>::type * = nullptr>
void solve(
  const emir::OpsInput &input, double tolerance, std::ostream &log_os,
  std::ostream &solution_os
) {
  T solver(input, tolerance);
  solver.solve(log_os);
  solution_os << solver;
}

/**
 * @brief Process the given instance file and output the solution and the log
 * in the files with the given names.
 *
 * @param instance_file_name The name of the file with the instance data.
 * @param output_file_name The name of the file where the solution will be
 * stored.
 * @param log_file_name The name of the file where the logs will be stored.
 * @param id The id of the instance to be processed.
 * @return 0 if the instance was processed successfully.
 */
int processor(
  const std::string &instance_file_name, const std::string &output_file_name,
  const std::string &log_file_name, const int id
) {
  std::ofstream log_file(log_file_name);
  std::ofstream output_file(output_file_name);
  const double tolerance = 1e-4;
  const auto input = createFromFile<emir::OpsInput>(instance_file_name);

  solve<emir::OpsCplexSolver>(input, tolerance, log_file, output_file);

  output_file.close();
  log_file.close();

  return 0;
}

#endif  // _SOLVERS_HPP_