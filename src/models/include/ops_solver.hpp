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

#include <ops_output.hpp>

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
   * @param tolerance The tolerance to be used in the solver.
   */
  OpsSolver(const OpsInput &input, const double tolerance);

  /**
   * @brief Assign the input and max tolerance to the solver.
   *
   * @param input A rvalue with the input of the solver that contains the
   * instance data.
   * @param tolerance The tolerance to be used in the solver.
   */
  OpsSolver(OpsInput &&input, const double tolerance);

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
  // The input of the solver with the instance data.
  const OpsInput input_;
  // The output where the solution will be stored.
  OpsOutput output_;
  // The tolerance to be used in the solver.
  const double tolerance_;
};

}  // namespace emir

#endif  // _EMIR_OPS_SOLVER_HPP_