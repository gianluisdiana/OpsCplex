/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file target_t.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.1.1
 * @date January 21, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of a 2 dimensional point with a
 * certain exposure time and priority.
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

#ifndef _EMIR_TARGET_HPP_
#define _EMIR_TARGET_HPP_

#include <iostream>

#include <json.hpp>
#include <point_t.hpp>

using json = nlohmann::json;

namespace emir {

class Target : public Point {
 private:
  double p_; /**< Exposure time */
  double b_; /**< Priority value */

 public:
  Target() : Point(), p_(0), b_(-1) {}

  Target(const Point &coord, double p, double b) : Point(coord), p_(p), b_(b) {}

  Target(const Point &coord) : Point(coord), p_(0), b_(-1) {}

  inline double get_p() const {
    return p_;
  }

  inline double get_b() const {
    return b_;
  }

  inline const Point &get_coord() const {
    return *this;
  }

  virtual ~Target() {}

  std::ostream &write(std::ostream &os) const;
  std::istream &read(std::istream &is);

  void get_json(json &target) const;
  void set_json(const json &target);
};

}  // namespace emir

#endif  // _EMIR_TARGET_HPP_