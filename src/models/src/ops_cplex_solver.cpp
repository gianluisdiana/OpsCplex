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
  OpsSolver(input, tolerance), cplex_(env_), model_(env_), x_(env_), y_(env_),
  s_(env_) {}

OpsCplexSolver::OpsCplexSolver(OpsInput &&input, const double tolerance) :
  OpsSolver(std::forward<OpsInput>(input), tolerance), cplex_(env_),
  model_(env_), x_(env_), y_(env_), s_(env_) {}

OpsCplexSolver::~OpsCplexSolver() {
  env_.end();
}

void OpsCplexSolver::solve() {
  makeModel();
  setParameters();
  cplex_.extract(model_);
  auto start_time = std::chrono::high_resolution_clock::now();
  decltype(start_time) current_time;
  try {
    cplex_.solve();
    current_time = std::chrono::high_resolution_clock::now();
  } catch (const IloException &ex) {
    std::cerr << "IloException: " << ex << '\n';
    return;
  } catch (...) {
    std::cerr << "Error" << '\n';
    return;
  }
  const auto &time_elapsed =
    std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time)
      .count();
  setOutput(time_elapsed);
}

// ------------------------------------------------------------------------- //
// ---------------------------- Private Methods ---------------------------- //
// ------------------------------------------------------------------------- //

void OpsCplexSolver::makeModel() {
  addYVariable();
  addSVariable();
  addXVariable();
  addObjective();
  addConstraints();
}

void OpsCplexSolver::addYVariable() {
  for (int j = 1; j < input_.getN() - 1; ++j) {
    y_.add(
      IloNumVar(env_, 0, 1, IloNumVar::Bool, std::format("y_{}", j).c_str())
    );
  }
  model_.add(y_);
}

void OpsCplexSolver::addSVariable() {
  for (int j = 0; j < input_.getN(); j++) {
    s_.add(IloNumVar(
      env_, 0, IloInfinity, IloNumVar::Float, std::format("s_{}", j).c_str()
    ));
  }
  model_.add(s_);
}

void OpsCplexSolver::addXVariable() {
  for (int k = 0; k < input_.getM(); ++k) {
    const auto graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &origin_id = arc.getOriginId();
      const auto &destination_id = arc.getDestinationId();
      x_.add(IloNumVar(
        env_, 0, 1, IloNumVar::Bool,
        std::format("x_{}_{}_{}", k + 1, origin_id, destination_id).c_str()
      ));
    }
  }
  model_.add(x_);
}

void OpsCplexSolver::addObjective() {
  IloExpr expression(env_);
  for (auto node_idx = 1; node_idx < input_.getN() - 1; ++node_idx) {
    expression += input_.getB(node_idx) * y_[node_idx - 1];
  }
  model_.add(IloMaximize(env_, expression));
  expression.end();
}

void OpsCplexSolver::addConstraints() {
  IloRangeArray constraints(env_);
  addDeltaPlusConstraints(constraints);
  addDeltaMinusConstraints(constraints);
  addMTZConstraints(constraints);
  addLimitConstraints(constraints);
  model_.add(constraints);
  constraints.end();
}

void OpsCplexSolver::addDeltaPlusConstraints(IloRangeArray &constraints) {
  for (auto k = 0; k < input_.getM(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &origin_id : graph.getNodesId()) {
      IloExpr expression(env_);
      const auto arcs_id = graph.getSuccessorsArcsId(origin_id);
      if (arcs_id.empty()) { continue; }
      for (const auto &arc_id : arcs_id) { expression += x_[arc_id]; }
      if (origin_id != 0) { expression -= y_[origin_id - 1]; }
      const double is_root_node = origin_id == 0 ? 1.0 : 0.0;
      constraints.add(IloRange(
        env_, is_root_node, expression, is_root_node,
        std::format("deltaplus_{}_{}", k + 1, origin_id).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addDeltaMinusConstraints(IloRangeArray &constraints) {
  const auto last_node_id = input_.getN() - 1;
  for (auto k = 0; k < input_.getM(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &node_id : graph.getNodesId()) {
      IloExpr expression(env_);
      const auto arcs_id = graph.getPredecessorsArcsId(node_id);
      if (arcs_id.empty()) { continue; }
      for (const auto &arc_id : arcs_id) { expression += x_[arc_id]; }
      if (node_id != last_node_id) { expression -= y_[node_id - 1]; }
      const double is_last_node = node_id == last_node_id ? 1.0 : 0.0;
      constraints.add(IloRange(
        env_, is_last_node, expression, is_last_node,
        std::format("deltaminus_{}_{}", k + 1, node_id).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addMTZConstraints(IloRangeArray &constraints) {
  const int BIG_M = std::max((int)input_.getMaxArc(), input_.getL()) + 1;
  for (auto k = 0; k < input_.getM(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &origin_id = arc.getOriginId();
      const auto &destination_id = arc.getDestinationId();
      IloExpr expression(env_);
      expression = BIG_M * x_[arc.getId()] + s_[origin_id] - s_[destination_id];
      constraints.add(IloRange(
        env_, -IloInfinity, expression,
        BIG_M - input_.getT(origin_id, destination_id),
        std::format("MTZ_{}_{}_{}", k + 1, origin_id, destination_id).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addLimitConstraints(IloRangeArray &constraints) {
  IloExpr start_time_expression(env_);
  start_time_expression = s_[0];
  constraints.add(IloRange(env_, 0, start_time_expression, 0, "Limit0"));
  start_time_expression.end();

  IloExpr end_time_expression(env_);
  end_time_expression = s_[(long)input_.getN() - 1];
  constraints.add(
    IloRange(env_, -IloInfinity, end_time_expression, input_.getL(), "Limit")
  );
  end_time_expression.end();
}

void OpsCplexSolver::setParameters() {
  cplex_.setParam(IloCplex::Param::TimeLimit, 3600);
  cplex_.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, tolerance_);
  cplex_.setParam(IloCplex::Param::Emphasis::MIP, CPX_MIPEMPHASIS_OPTIMALITY);
}

void OpsCplexSolver::setOutput(long time_elapsed) {
  const auto used_arcs = IloNumVarArrayToVector(x_);
  const auto visited_objects = IloNumVarArrayToVector(y_);
  auto time_at_objects = IloNumVarArrayToVector(s_);
  for (int i = 1; i < time_at_objects.size() - 1; ++i) {
    time_at_objects[i] = time_at_objects[i] * visited_objects[i - 1];
  }
  output_.setX(used_arcs);
  output_.setY(visited_objects);
  output_.setS(time_at_objects);
  output_.setTimeSpent(time_elapsed);
}

std::vector<double>
OpsCplexSolver::IloNumVarArrayToVector(const IloNumVarArray &variable) const {
  IloNumArray values(env_);
  cplex_.getValues(variable, values);
  std::vector<double> result(values.getSize());
  for (auto i = 0; i < values.getSize(); ++i) { result[i] = values[i]; }
  return result;
}

}  // namespace emir

// NOLINTEND(misc-include-cleaner)