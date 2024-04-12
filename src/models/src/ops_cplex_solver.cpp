// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_cplex_solver.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the solver for the O.P.S. problem
 * using CPLEX library from IBM.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <vector>

#include <ops_cplex_solver.hpp>
#include <ops_input.hpp>
#include <ops_solver.hpp>

// NOLINTBEGIN(misc-include-cleaner)
namespace emir {

OpsCplexSolver::OpsCplexSolver(const OpsInput &input, const double tolerance) :
  OpsSolver(input, tolerance), cplex_(environment_), model_(environment_),
  used_arcs_(environment_), observed_objects_(environment_),
  time_at_objects_(environment_) {}

OpsCplexSolver::OpsCplexSolver(OpsInput &&input, const double tolerance) :
  OpsSolver(std::forward<OpsInput>(input), tolerance), cplex_(environment_),
  model_(environment_), used_arcs_(environment_),
  observed_objects_(environment_), time_at_objects_(environment_) {}

OpsCplexSolver::~OpsCplexSolver() {
  environment_.end();
}

void OpsCplexSolver::solve() {
  makeModel();
  setParameters();
  cplex_.extract(model_);
  try {
    timer_.reset();
    cplex_.solve();
    setOutput(timer_.elapsed<std::chrono::milliseconds>());
  } catch (const IloException &ex) {
    std::cerr << "IloException: " << ex << '\n';
    return;
  } catch (...) {
    std::cerr << "Error" << '\n';
    return;
  }
}

// ---------------------------- Private Methods ---------------------------- //

void OpsCplexSolver::makeModel() {
  addYVariable();
  addSVariable();
  addXVariable();
  addObjective();
  addConstraints();
}

// -------------------------------- Adders -------------------------------- //

void OpsCplexSolver::addYVariable() {
  for (int j = 1; j < input_.getAmountOfObjects() - 1; ++j) {
    observed_objects_.add(IloNumVar(
      environment_, 0, 1, IloNumVar::Bool, std::format("y_{}", j).c_str()
    ));
  }
  model_.add(observed_objects_);
}

void OpsCplexSolver::addSVariable() {
  for (int j = 0; j < input_.getAmountOfObjects(); j++) {
    time_at_objects_.add(IloNumVar(
      environment_, 0, IloInfinity, IloNumVar::Float,
      std::format("s_{}", j).c_str()
    ));
  }
  model_.add(time_at_objects_);
}

void OpsCplexSolver::addXVariable() {
  for (int k = 0; k < input_.getAmountOfSlidingBars(); ++k) {
    const auto graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &origin_id = arc.getOriginId();
      const auto &destination_id = arc.getDestinationId();
      used_arcs_.add(IloNumVar(
        environment_, 0, 1, IloNumVar::Bool,
        std::format("x_{}_{}_{}", k + 1, origin_id, destination_id).c_str()
      ));
    }
  }
  model_.add(used_arcs_);
}

void OpsCplexSolver::addObjective() {
  IloExpr expression(environment_);
  for (auto node_idx = 1; node_idx < input_.getAmountOfObjects() - 1;
       ++node_idx) {
    expression +=
      input_.getPriority(node_idx) * observed_objects_[node_idx - 1];
  }
  model_.add(IloMaximize(environment_, expression));
  expression.end();
}

void OpsCplexSolver::addConstraints() {
  IloRangeArray constraints(environment_);
  addDeltaPlusConstraints(constraints);
  addDeltaMinusConstraints(constraints);
  addMTZConstraints(constraints);
  addLimitConstraints(constraints);
  model_.add(constraints);
  constraints.end();
}

void OpsCplexSolver::addDeltaPlusConstraints(IloRangeArray &constraints) {
  for (auto k = 0; k < input_.getAmountOfSlidingBars(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &origin_id : graph.getNodesId()) {
      IloExpr expression(environment_);
      const auto arcs_id = graph.getSuccessorsArcsId(origin_id);
      if (arcs_id.empty()) { continue; }
      for (const auto &arc_id : arcs_id) { expression += used_arcs_[arc_id]; }
      if (origin_id != 0) { expression -= observed_objects_[origin_id - 1]; }
      const double is_root_node = origin_id == 0 ? 1.0 : 0.0;
      constraints.add(IloRange(
        environment_, is_root_node, expression, is_root_node,
        std::format("deltaplus_{}_{}", k + 1, origin_id).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addDeltaMinusConstraints(IloRangeArray &constraints) {
  const auto last_node_id = input_.getAmountOfObjects() - 1;
  for (auto k = 0; k < input_.getAmountOfSlidingBars(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &node_id : graph.getNodesId()) {
      IloExpr expression(environment_);
      const auto arcs_id = graph.getPredecessorsArcsId(node_id);
      if (arcs_id.empty()) { continue; }
      for (const auto &arc_id : arcs_id) { expression += used_arcs_[arc_id]; }
      if (node_id != last_node_id) {
        expression -= observed_objects_[node_id - 1];
      }
      const double is_last_node = node_id == last_node_id ? 1.0 : 0.0;
      constraints.add(IloRange(
        environment_, is_last_node, expression, is_last_node,
        std::format("deltaminus_{}_{}", k + 1, node_id).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addMTZConstraints(IloRangeArray &constraints) {
  const int BIG_M =
    std::max((int)input_.getMaxArc(), input_.getTimeLimit()) + 1;
  for (auto k = 0; k < input_.getAmountOfSlidingBars(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &origin_id = arc.getOriginId();
      const auto &destination_id = arc.getDestinationId();
      IloExpr expression(environment_);
      expression = BIG_M * used_arcs_[arc.getId()] +
                   time_at_objects_[origin_id] -
                   time_at_objects_[destination_id];
      constraints.add(IloRange(
        environment_, -IloInfinity, expression,
        BIG_M - input_.getTimeToProcess(origin_id, destination_id),
        std::format("MTZ_{}_{}_{}", k + 1, origin_id, destination_id).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addLimitConstraints(IloRangeArray &constraints) {
  IloExpr start_time_expression(environment_);
  start_time_expression = time_at_objects_[0];
  constraints.add(IloRange(environment_, 0, start_time_expression, 0, "Limit0")
  );
  start_time_expression.end();

  IloExpr end_time_expression(environment_);
  end_time_expression = time_at_objects_[(long)input_.getAmountOfObjects() - 1];
  constraints.add(IloRange(
    environment_, -IloInfinity, end_time_expression, input_.getTimeLimit(),
    "Limit"
  ));
  end_time_expression.end();
}

// -------------------------------- Setters -------------------------------- //

void OpsCplexSolver::setParameters() {
  cplex_.setParam(IloCplex::Param::TimeLimit, 3600);
  cplex_.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, tolerance_);
  cplex_.setParam(IloCplex::Param::Emphasis::MIP, CPX_MIPEMPHASIS_OPTIMALITY);
}

void OpsCplexSolver::setOutput(long time_elapsed) {
  const auto used_arcs = IloNumVarArrayToVector(used_arcs_);
  const auto visited_objects = IloNumVarArrayToVector(observed_objects_);
  auto time_at_objects = IloNumVarArrayToVector(time_at_objects_);
  for (int i = 1; i < time_at_objects.size() - 1; ++i) {
    time_at_objects[i] = time_at_objects[i] * visited_objects[i - 1];
  }
  output_.setUsedArcs(used_arcs);
  output_.setObservedObjects(visited_objects);
  output_.setTimeAtObjects(time_at_objects);
  output_.setTimeSpent(time_elapsed);
}

// -------------------------------- Utility -------------------------------- //

std::vector<double>
OpsCplexSolver::IloNumVarArrayToVector(const IloNumVarArray &variable) const {
  IloNumArray values(environment_);
  cplex_.getValues(variable, values);
  std::vector<double> result(values.getSize());
  for (auto i = 0; i < values.getSize(); ++i) { result[i] = values[i]; }
  return result;
}

}  // namespace emir

// NOLINTEND(misc-include-cleaner)