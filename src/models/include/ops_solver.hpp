// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_solver.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the general structure of the solver for the O.P.S.
 * problem. The solver must contains the input and output, and the method to
 * solve the problem.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef _EMIR_OPS_SOLVER_HPP_
#define _EMIR_OPS_SOLVER_HPP_

#include <local_type_traits.hpp>
#include <ops_output.hpp>
#include <timer.hpp>

namespace emir {

/**
 * @brief The base class with the input and output of the algorithm that
 * solves the O.P.S. problem.
 */
class OpsSolver {
 public:
  /**
   * @brief Assign the input and max tolerance to the solver.
   *
   * @param input A lvalue with the input of the solver that contains the
   * instance data.
   */
  explicit OpsSolver(const OpsInput &input);

  /**
   * @brief Assign the input and max tolerance to the solver.
   *
   * @param input A rvalue with the input of the solver that contains the
   * instance data.
   */
  explicit OpsSolver(OpsInput &&input);

  /** @brief Destroy the OpsSolver object. */
  virtual ~OpsSolver() = default;

  // ------------------------------- Utility ------------------------------- //

  /**
   * @brief Solve the O.P.S. problem and store the solution in the output.
   */
  virtual void solve() = 0;

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the << operator to print the output of the solver.
   *
   * @param output_stream Represents the outflow.
   * @param solver The solver to print the output.
   * @return The outflow with the output in a JSON format.
   */
  friend std::ostream &
  operator<<(std::ostream &output_stream, const OpsSolver &solver);

 protected:
  // ---------------------------- Input Methods ---------------------------- //

  /** @brief Get the input of the solver. */
  [[nodiscard]] const OpsInput &getInput() const {
    return input_;
  }

  // --------------------------- Output Methods ---------------------------- //

  /**
   * @brief Assign the used arcs to the output.
   *
   * @param used_arcs The used arcs in the solution.
   */
  void setUsedArcsToOutput(const std::vector<double> &used_arcs) {
    output_.setUsedArcs(used_arcs);
  }

  /**
   * @brief Assign the observed objects to the output.
   *
   * @param observed_objects The observed objects in the solution.
   */
  void setObservedObjectsToOutput(const std::vector<double> &observed_objects) {
    output_.setObservedObjects(observed_objects);
  }

  /**
   * @brief Assign the time spent at objects to the output.
   *
   * @param time_at_objects The time spent at objects in the solution.
   */
  void setTimeAtObjectsToOutput(const std::vector<double> &time_at_objects) {
    output_.setTimeAtObjects(time_at_objects);
  }

  /**
   * @brief Assign the time spent to the output.
   *
   * @param time_spent The time spent in the solution.
   */
  void setTimeSpentToOutput(const long time_spent) {
    output_.setTimeSpent(time_spent);
  }

  // ---------------------------- Timer Methods ---------------------------- //

  /** @brief Reset the timer with the current time. */
  void resetTimer() {
    timer_.reset();
  }

  /**
   * @brief Get the elapsed time since the last reset.
   *
   * @tparam TimeUnit The time unit to use for the elapsed time.
   * @return The elapsed time in the specified time unit.
   */
  template <typename TimeUnit>
  requires is_time_unit_v<TimeUnit>
  [[nodiscard]] auto getElapsedTime() const -> decltype(TimeUnit {}.count()) {
    return timer_.elapsed<TimeUnit>();
  }

 private:
  // ------------------------------ Attributes ----------------------------- //

  // The input of the solver with the instance data.
  OpsInput input_;
  // The output where the solution will be stored.
  OpsOutput output_;
  // The timer to measure the elapsed time.
  Timer timer_;
};

}  // namespace emir

#endif  // _EMIR_OPS_SOLVER_HPP_