#ifndef _EMIR_OPS_BC1_HPP_
#define _EMIR_OPS_BC1_HPP_

#include <ilcplex/ilocplex.h>

#include <ops_solver.hpp>

namespace emir {

class OpsCplexSolver : public OpsSolver {
 public:
  OpsCplexSolver(const OpsInput &input, double eps);
  ~OpsCplexSolver();

  void solve(std::ostream &r_os) override;

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

  /** @brief Set the output with the models solution. */
  void setOutput();

  /**
   * @brief Gets the values from the cplex solver and converts them.
   *
   * @param variable The variable to get the values from.
   * @return The values from the variable in a vector.
   */
  const std::vector<double>
  IloNumVarArrayToVector(const IloNumVarArray &variable) const;
};

}  // namespace emir

#endif  // _EMIR_OPS_BC1_HPP_