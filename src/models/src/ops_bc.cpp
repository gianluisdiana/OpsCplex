#include <ops_bc.hpp>

namespace emir {

OpsCplexSolver::OpsCplexSolver(const OpsInput &input, double eps) :
  OpsSolver(input, eps), env_(), cplex_(env_), model_(env_), x_(env_), y_(env_),
  s_(env_) {}

OpsCplexSolver::~OpsCplexSolver() {
  env_.end();
}

void OpsCplexSolver::solve(std::ostream &r_os) {
  try {
    makeModel();
    setParameters();
    cplex_.setOut(r_os);
    cplex_.extract(model_);
    cplex_.solve();
  } catch (IloException &ex) {
    std::cerr << "Error: " << ex << '\n';
    return;
  } catch (...) {
    std::cerr << "Error" << '\n';
    return;
  }
  setOutput();
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
      IloNumVar(env_, 0, 1, IloNumVar::Bool, ("y_" + std::to_string(j)).c_str())
    );
  }
  model_.add(y_);
}

void OpsCplexSolver::addSVariable() {
  for (int j = 0; j < input_.getN(); j++) {
    s_.add(IloNumVar(
      env_, 0, IloInfinity, IloNumVar::Float, ("s_" + std::to_string(j)).c_str()
    ));
  }
  model_.add(s_);
}

void OpsCplexSolver::addXVariable() {
  for (int k = 0; k < input_.getM(); ++k) {
    const auto graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &origin_node = arc.getOriginId();
      const auto &destination_node = arc.getDestinationId();
      x_.add(IloNumVar(
        env_, 0, 1, IloNumVar::Bool,
        ("x_" + std::to_string(k + 1) + "_" + origin_node + "_" +
         destination_node)
          .c_str()
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
    for (auto node_idx = 0; node_idx < input_.getN() - 1; ++node_idx) {
      const auto &node_string = std::to_string(node_idx);
      const auto mki = input_.getAmountOfSuccessors(k, node_string);
      if (mki == 0) continue;

      IloExpr expression(env_);
      for (auto l = 0; l < mki; ++l) {
        expression += x_[input_.getArcId(k, node_string, l, true)];
      }
      if (node_idx > 0) expression -= y_[node_idx - 1];

      const double is_root_node = node_idx == 0 ? 1.0 : 0.0;
      constraints.add(IloRange(
        env_, is_root_node, expression, is_root_node,
        ("deltaplus_" + node_string + "_" + std::to_string(k + 1)).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addDeltaMinusConstraints(IloRangeArray &constraints) {
  for (auto k = 0; k < input_.getM(); ++k) {
    for (auto node_idx = 1; node_idx < input_.getN(); ++node_idx) {
      const auto &node_string = std::to_string(node_idx);
      const auto mki = input_.getAmountOfPredecessors(k, node_string);
      if (mki == 0) continue;

      IloExpr expression(env_);
      for (auto l = 0; l < mki; ++l)
        expression += x_[input_.getArcId(k, node_string, l, false)];
      if (node_idx < input_.getN() - 1) expression -= y_[node_idx - 1];

      const double is_last_node = node_idx == (input_.getN() - 1) ? 1.0 : 0.0;
      constraints.add(IloRange(
        env_, is_last_node, expression, is_last_node,
        ("deltaminus_" + node_string + '_' + std::to_string(k + 1)).c_str()
      ));
      expression.end();
    }
  }
}

void OpsCplexSolver::addMTZConstraints(IloRangeArray &constraints) {
  const int BIG_M = std::max<int>(input_.getMaxArc(), input_.getL()) + 1;
  for (auto k = 0, x_idx = 0; k < input_.getM(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &i = arc.getOriginId();
      const auto &j = arc.getDestinationId();
      IloExpr expression(env_);
      expression = BIG_M * x_[x_idx] + s_[std::stoi(i)] - s_[std::stoi(j)];

      constraints.add(IloRange(
        env_, -IloInfinity, expression,
        BIG_M - input_.getT(std::stoi(i), std::stoi(j)),
        ("MTZ_" + i + "_" + j + "_" + std::to_string(k + 1)).c_str()
      ));
      expression.end();
      x_idx++;
    }
  }
}

void OpsCplexSolver::addLimitConstraints(IloRangeArray &constraints) {
  IloExpr start_time_expression(env_);
  start_time_expression = s_[0];
  constraints.add(IloRange(env_, 0, start_time_expression, 0, "Limit0"));
  start_time_expression.end();

  IloExpr last_time_expression(env_);
  last_time_expression = s_[input_.getN() - 1];
  constraints.add(
    IloRange(env_, -IloInfinity, last_time_expression, input_.getL(), "Limit")
  );
  last_time_expression.end();
}

void OpsCplexSolver::setParameters() {
  cplex_.setParam(IloCplex::Param::TimeLimit, 3600);
  cplex_.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, 1e-3);
  cplex_.setParam(IloCplex::Param::Emphasis::MIP, CPX_MIPEMPHASIS_OPTIMALITY);
}

void OpsCplexSolver::setOutput() {
  const auto x = IloNumVarArrayToVector(x_);
  const auto y = IloNumVarArrayToVector(y_);
  auto s = IloNumVarArrayToVector(s_);
  for (int i = 1; i < s.size() - 1; ++i) s[i] = s[i] * y[i - 1];
  output_.set(x, y, s);
}

const std::vector<double>
OpsCplexSolver::IloNumVarArrayToVector(const IloNumVarArray &variable) const {
  IloNumArray values(env_);
  cplex_.getValues(variable, values);
  std::vector<double> result;
  for (auto i = 0; i < values.getSize(); ++i) { result.push_back(values[i]); }
  return result;
}

}  // namespace emir