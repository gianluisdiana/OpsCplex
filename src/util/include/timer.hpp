// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file timer.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 14, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description and implementation of a basic timer to
 * measure elapsed time.
 * It uses the C++ standard library chrono to measure in a simple and efficient
 * way.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

#include <local_type_traits.hpp>

/** @brief A simple timer class to measure elapsed time. */
class Timer {
 public:
  /** @brief Construct a new Timer object with the current time. */
  Timer() {
    reset();
  }

  /** @brief Reset the timer to the current time. */
  void reset() {
    start_ = now();
  }

  /**
   * @brief Get the elapsed time since the last reset.
   *
   * @tparam TimeUnit The time unit to use for the elapsed time.
   * @return The elapsed time in seconds.
   */
  template <typename TimeUnit>
  requires is_time_unit_v<TimeUnit>
  [[nodiscard]] auto elapsed() const -> decltype(TimeUnit {}.count()) {
    return std::chrono::duration_cast<TimeUnit>(now() - start_).count();
  }

 private:
  // ----------------------------- Attributes ----------------------------- //

  // The time point when the timer was started.
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;

  // --------------------------- Private Methods --------------------------- //

  /** @brief Get the current time. */
  static std::chrono::time_point<std::chrono::high_resolution_clock> now() {
    return std::chrono::high_resolution_clock::now();
  }
};

#endif  // TIMER_HPP_