/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file point_t.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.1.1
 * @date January 21, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of a basic point in a 2
 * dimensional space.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex
 * @endlink
 * @see Selective routing problem with synchronization @link
 * https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1
 * @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link
 * https://google.github.io/styleguide/cppguide.html @endlink
 */

#ifndef _EMIR_POINT_HPP_
#define _EMIR_POINT_HPP_

#include <iomanip>
#include <iostream>
#include <utility>

#include <json.hpp>

using json = nlohmann::json;

namespace emir {

class Point : public std::pair<double, double> {
 public:
  Point(void) : std::pair<double, double>(0, 0) {}

  Point(double a, double b) : std::pair<double, double>(a, b) {}

  virtual ~Point(void) {}

  inline const double get_x(void) const {
    return first;
  }

  inline const double get_y(void) const {
    return second;
  }
};

}  // namespace emir

#endif  // _EMIR_POINT_HPP_