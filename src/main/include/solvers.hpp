#ifndef _SOLVERS_HPP_
#define _SOLVERS_HPP_

#include <ops_solver.hpp>

/**
 * @brief Solve the O.P.S. problem using the given solver.
 * Also output the log and the solution in the given outflows.
 *
 * @tparam T The type of the solver to be used. It can be any class that
 * inherits from emir::OpsSolver.
 * @param input The input of the solver with the instance data.
 * @param tolerance The maximum tolerance to be used in the solver.
 * @param log_os The outflow where the logs will be stored.
 */
template <
  typename T, typename std::enable_if<
                std::is_base_of<emir::OpsSolver, T>::value>::type * = nullptr>
T solve(const emir::OpsInput &input, double tolerance, std::ostream &log_os) {
  T solver(input, tolerance);
  solver.addLog(log_os);
  solver.solve();
  return solver;
}

/**
 * @brief Process the model type and generates the output and log files.
 *
 * @param model_type The model type to be processed.
 */
void processModelType(const std::string &model_type);

/**
 * @brief Process the input file and generates the output and log files.
 *
 * @param inputPath The path to the input file to be processed.
 */
void processInputFile(const std::string &inputPath);

#endif  // _SOLVERS_HPP_