#ifndef _SOLVERS_HPP_
#define _SOLVERS_HPP_

#include <functions.hpp>
#include <ops_solver.hpp>

/**
 * @brief Solve the O.P.S. problem using the given solver.
 * Also output the log in the given outflow.
 *
 * @tparam Solver The type of the solver to be used. It can be any class that
 * inherits from emir::OpsSolver.
 * @param input_path The name of the file that has the configuration of the
 * O.P.S input.
 * @param tolerance The maximum tolerance to be used in the solver.
 * @param log_os The outflow where the logs will be stored.
 */
template <typename Solver>
Solver solve(
  const std::string &input_path, const double tolerance, std::ostream &log_os
) requires std::is_base_of_v<emir::OpsSolver, Solver> {
  const auto &instance = createFromFile<emir::OpsInput>(input_path);
  Solver solver(instance, tolerance);
  solver.addLog(log_os);
  solver.solve();
  return solver;
}

/**
 * @brief Creates an output file with the solution of the given input file.
 *
 * @param input_path The path to the input file to be processed.
 * @param output_path The path to the output file to be generated.
 */
void processInstance(
  const std::string &input_path, const std::string &output_path
);

/**
 * @brief Process the model type and generates the output and log files.
 *
 * @param model_type The model type to be processed.
 */
void processModelType(const std::string &model_type);

/**
 * @brief Process the input file and generates the output and log files.
 *
 * @param input_path The path to the input file to be processed.
 */
void processFile(const std::string &input_path);

#endif  // _SOLVERS_HPP_