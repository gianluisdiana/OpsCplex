// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file functions.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of some useful functions used
 * throughout the project.
 * `stringToDateStamp` converts a string to a date stamp.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <functions.hpp>

std::time_t stringToDateStamp(const std::string &date_stamp) {
  struct std::tm time {};

  std::istringstream string_stream(date_stamp);
  string_stream >> std::get_time(&time, "%c");
  return std::mktime(&time);
}