// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file stats.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.2.0
 * @date February 5, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description and implementation from a general
 *  structure to store basic statistics (maximum, minimum, average, sum).
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef _EMIR_STATS_HPP_
#define _EMIR_STATS_HPP_

namespace emir {

/** @brief Represents general statistics of the algorithm. */
struct Stats {
  // Number of objects taken into account
  unsigned int amount;
  // Maximum amount of objects taken into account
  unsigned int max_amount;
  // Minimum amount of objects taken into account
  unsigned int min_amount;
  // Sum of each object's amount
  unsigned int amount_sum;

  /**
   * @brief Construct a new Stats object with the given minimum placeholder.
   *
   * @param minimum_placeholder The minimum placeholder for the minimum amount
   * of objects taken into account
   */
  Stats(unsigned int minimum_placeholder) :
    amount(0), max_amount(0), min_amount(minimum_placeholder), amount_sum(0) {}

  /** @brief Gets the average amount of objects taken into account */
  inline double getAvgAmount() const {
    return (double)amount_sum / (double)amount;
  }

  /**
   * @brief Update the maximum amount of objects taken into account (only if it
   * is greater than the current maximum).
   *
   * @param current_amount The amount of objects
   */
  inline void updateMax(unsigned int current_amount) {
    if (current_amount > max_amount) max_amount = current_amount;
  }

  /**
   * @brief Update the minimum amount of objects taken into account (only if it
   * is lower than the current minimum).
   *
   * @param current_amount The amount of objects
   */
  inline void updateMin(unsigned int current_amount) {
    if (current_amount < this->min_amount) this->min_amount = current_amount;
  }
};

}  // namespace emir

#endif  // _EMIR_STATS_HPP_