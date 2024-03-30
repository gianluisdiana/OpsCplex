#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <ctime>
#include <fstream>

#include <local_type_traits.hpp>
#include <ops_solver.hpp>

/**
 * @brief Converts a string to a date stamp.
 *
 * @param date_stamp String to be converted.
 * @return The date stamp of the string.
 */
time_t stringToDateStamp(const std::string &date_stamp);

/**
 * @brief Import the data from a file and convert it to a given type.
 *
 * @tparam Readable Type to be converted to.
 * @param file_name Name of the file to be imported.
 * @return The data from the file converted to the given type.
 */
template <typename Readable>
Readable createFromFile(const std::string &file_name
) requires is_readable_v<Readable &> && std::default_initializable<Readable> {
  std::ifstream file(file_name);
  Readable data;
  file >> data;
  return data;
}

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

#endif  // _FUNCTIONS_HPP_