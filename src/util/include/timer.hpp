#ifndef TIMER_HPP
#define TIMER_HPP

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

#endif  // TIMER_HPP