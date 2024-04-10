// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_cplex_solver.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of the solver for the O.P.S. problem
 * using CPLEX library from IBM.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef _EMIR_OPS_CPLEX_SOLVER_HPP_
#define _EMIR_OPS_CPLEX_SOLVER_HPP_

#include <ilcplex/ilocplex.h>

#include <ops_solver.hpp>

namespace emir {

/**
 * @brief Implements the algorithm that solves the O.P.S. problem using a
 * CPLEX.
 */
class OpsCplexSolver : public OpsSolver {
 public:
  /**
   * @brief Assign the input and max tolerance to the solver.
   *
   * @param input The input of the solver with the instance data.
   * @param tolerance The tolerance to be used in the solver.
   */
  OpsCplexSolver(const OpsInput &input, const double tolerance);

  /**
   * @brief Moves the input and assigns the max tolerance to the solver.
   *
   * @param input The input of the solver with the instance data.
   * @param tolerance The tolerance to be used in the solver.
   */
  OpsCplexSolver(OpsInput &&input, const double tolerance);

  /** @brief Delete the environment of the solution. */
  ~OpsCplexSolver();

  /**
   * @brief Solve the Linear Programming problem.
   * The problem is solved using the CPLEX solver.
   */
  void solve() override;

  /** @brief Get the profit of the solution. */
  inline int getProfit() const {
    return cplex_.getObjValue();
  }

  /**
   * @brief Add a log stream to output the log of the solver.
   *
   * @param log_os The output stream to write the logs to.
   */
  inline void addLog(std::ostream &log_os) {
    cplex_.setOut(log_os);
  }

 private:
  // An environment, manage the memory and identifiers for modeling objects.
  IloEnv env_;
  // Algorithm used to solve the Linear Programming problem.
  IloCplex cplex_;
  // Model that represents the Linear Programming problem.
  IloModel model_;

  // --------------------------- Model Variables --------------------------- //

  // Binary vector (1 and 0) that indicates whether an arc is beeing used or
  // not.
  IloNumVarArray x_;
  // Binary vector (1 and 0) that indicates whether an object is beeing observed
  // or not.
  IloNumVarArray y_;
  // Float vector that stores how much time has passed since the beginning of
  // the observation to the moment the object is observed.
  IloNumVarArray s_;

  /** @brief Creates the model for the problem, using the input data. */
  void makeModel();

  /**
   * @brief Add the 'y' variables to the model.
   * Y is a binary vector of length n (number of objects in the problem), that
   * indicates whether an object is beeing observed or not.
   *
   * @param model The model to add the variables to.
   */
  void addYVariable();

  /**
   * @brief Add the 's' variables to the model.
   * S is a float vector of length n + 2 (number of objects in the problem plus
   * the initial and final nodes), that stores how much time has passed since
   * the beginning of the observation to the moment the object is observed.
   *
   * @param model The model to add the variables to.
   */
  void addSVariable();

  /**
   * @brief Add the 'x' variables to the model.
   * X is a binary vector which length depends on the number of arcs in the
   * problem. It indicates whether an arc is beeing used or not.
   *
   * @param model The model to add the variables to.
   */
  void addXVariable();

  /**
   * @brief Add what is the objective equation to the model.
   * In this case, the objective is to maximize the number of objects observed
   * (based on its priority).
   *
   * @param model The model to add the constraints to.
   */
  void addObjective();

  /**
   * @brief Add all the constraints to the model.
   * The constraints are:
   * 1. The root node only has one arc leaving it.
   * 2. The final node only has one arc entering it.
   * 3. The amount of arcs that leave a node is equal to the amount of arcs that
   * enter the node.
   * 4. Only one arc can enter / leave a node.
   * 5. The time spent to process an object minus the time spent to process the
   * previous object must be lower than the total time available.
   *
   * @param model The model to add the constraints to.
   */
  void addConstraints();

  /**
   * @brief Add the delta plus constraints to the model.
   * The delta plus constraints are the ones that ensure that the amount of arcs
   * that leave a node is equal to 1.
   *
   * @param constraints The constraints to add the delta plus constraints to.
   */
  void addDeltaPlusConstraints(IloRangeArray &constraints);

  /**
   * @brief Add the delta minus constraints to the model.
   * The delta minus constraints are the ones that ensure that the amount of
   * arcs that enter a node is equal to 1.
   *
   * @param constraints The constraints to add the delta minus constraints to.
   */
  void addDeltaMinusConstraints(IloRangeArray &constraints);

  /**
   * @brief Add the MTZ constraints to the model.
   * The MTZ constraints are the ones that ensure that the time spent to process
   * an object minus the time spent to process the previous object must be lower
   * than the total time available.
   *
   * @param constraints The constraints to add the MTZ constraints to.
   */
  void addMTZConstraints(IloRangeArray &constraints);

  /**
   * @brief Add the limit constraints to the model.
   * This constraint ensures that the starting time is zero and the ending time
   * is at most the total time available.
   *
   * @param constraints The constraints to add the limit constraints to.
   */
  void addLimitConstraints(IloRangeArray &constraints);

  /**
   * @brief Set some configuration parameters to the CPLEX solver.
   * - The maximum time to solve the problem is 3600 seconds.
   * - The absolute tolerance on the gap between the best integer objective and
   * the objective of the best node remaining is 0.001.
   * - Emphasize optimality over feasibility.
   */
  void setParameters();

  /**
   * @brief Set the output with the models solution
   *
   * @param time_elapsed The time spent to solve the problem.
   */
  void setOutput(long time_elapsed);

  /**
   * @brief Gets the values from the cplex solver and converts them.
   *
   * @param variable The variable to get the values from.
   * @return The values from the variable in a vector.
   */
  std::vector<double>
  IloNumVarArrayToVector(const IloNumVarArray &variable) const;
};

}  // namespace emir

#endif  // _EMIR_OPS_CPLEX_SOLVER_HPP_