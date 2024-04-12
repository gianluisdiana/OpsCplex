// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file functions.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of some useful functions used
 * throughout the project.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

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
std::time_t stringToDateStamp(const std::string &date_stamp);

/**
 * @brief Import the data from a file and convert it to a given type.
 *
 * @tparam Readable Type to be converted to.
 * @param file_name Name of the file to be imported.
 * @return The data from the file converted to the given type.
 */
template <typename Readable>
requires is_readable_v<Readable &> && std::default_initializable<Readable>
Readable createFromFile(const std::string &file_name) {
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
requires std::is_base_of_v<emir::OpsSolver, Solver>
Solver solve(
  const std::string &input_path, const double tolerance, std::ostream &log_os
) {
  Solver solver(createFromFile<emir::OpsInput>(input_path), tolerance);
  solver.addLog(log_os);
  solver.solve();
  return solver;
}

/**
 *@brief Compare two floating point numbers.
 *
 * @param first_number The first value to be compared.
 * @param second_number The second value to be compared.
 * @param tolerance The maximum tolerance to be used in the comparison.
 */
bool isEqual(
  const std::floating_point auto first_number,
  const std::floating_point auto second_number, const double tolerance = 1e-9
) {
  return std::abs(first_number - second_number) < tolerance;
}

#endif  // _FUNCTIONS_HPP_