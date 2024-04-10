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

  /** @brief Empty destructor */
  virtual ~OpsSolver() {}

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