#include <OPS_bc1.hpp>

namespace emir {

OPS_cplex_solver1::OPS_cplex_solver1(
  const OpsInput *I, OPS_output_t &O, double eps
) :
  OPS_solver_t(I, O, eps),
  env_(), x_(env_), y_(env_), s_(env_), cplex_(env_) {}

OPS_cplex_solver1::~OPS_cplex_solver1() {
  env_.end();
}

void OPS_cplex_solver1::set_param(std::ostream &r_os) {
  cplex_.setParam(IloCplex::Param::TimeLimit, 3600);

  cplex_.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, 1E-3);
  cplex_.setParam(IloCplex::Param::Emphasis::MIP, CPX_MIPEMPHASIS_OPTIMALITY);

  cplex_.setOut(r_os);
}

void OPS_cplex_solver1::solve(std::ostream &r_os, double ub, bool root_node) {
  try {
    auto model = makeModel();
    set_param(r_os);
    cplex_.extract(model);
    std::cout << model;
    cplex_.solve();
  } catch (IloException &ex) {
    std::cerr << "Error: " << ex << '\n';
    return;
  } catch (...) {
    std::cerr << "Error" << '\n';
    return;
  }
  set_output(O_);
}

void OPS_cplex_solver1::set_output(OPS_output_t &output) {
  IloNumArray x(env_);
  cplex_.getValues(x_, x);

  IloNumArray y(env_);
  cplex_.getValues(y_, y);

  IloNumArray s(env_);
  cplex_.getValues(s_, s);

  std::vector<double> xv(x.getSize());

  for (int i = x.getSize() - 1; i >= 0; i--) xv[i] = x[i];

  std::vector<double> yv(y.getSize());

  for (int i = y.getSize() - 1; i >= 0; i--) yv[i] = y[i];

  std::vector<double> sv(s.getSize());

  sv[0] = s[0];
  sv[s.getSize() - 1] = s[s.getSize() - 1];

  for (int i = s.getSize() - 2; i > 0; i--) sv[i] = s[i] * y[i - 1];

  output.set(xv, yv, sv);
}

// ------------------------------------------------------------------------- //
// ---------------------------- Private Methods ---------------------------- //
// ------------------------------------------------------------------------- //

IloModel OPS_cplex_solver1::makeModel() {
  IloModel model(env_);
  addYVariable(model);
  addSVariable(model);
  addXVariable(model);
  addObjective(model);
  addConstraints(model);
  return model;
}

void OPS_cplex_solver1::addYVariable(IloModel &model) {
  for (int j = 1; j < I_->getN() - 1; ++j) {
    y_.add(
      IloNumVar(env_, 0, 1, IloNumVar::Bool, ("y_" + std::to_string(j)).c_str())
    );
  }
  model.add(y_);
}

void OPS_cplex_solver1::addSVariable(IloModel &model) {
  for (int j = 0; j < I_->getN(); j++) {
    s_.add(IloNumVar(
      env_, 0, IloInfinity, IloNumVar::Float, ("s_" + std::to_string(j)).c_str()
    ));
  }
  model.add(s_);
}

void OPS_cplex_solver1::addXVariable(IloModel &model) {
  for (int k = 0; k < I_->getM(); ++k) {
    const auto graph = I_->getGraph(k);
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
  model.add(x_);
}

void OPS_cplex_solver1::addObjective(IloModel &model) {
  IloExpr expression(env_);
  for (auto node_idx = 1; node_idx < I_->getN() - 1; ++node_idx) {
    expression += I_->getB(node_idx) * y_[node_idx - 1];
  }
  model.add(IloMaximize(env_, expression));
  expression.end();
}

void OPS_cplex_solver1::addConstraints(IloModel &model) {
  IloRangeArray constraints(env_);
  addDeltaPlusConstraints(constraints);
  addDeltaMinusConstraints(constraints);
  addMTZConstraints(constraints);
  addLimitConstraints(constraints);
  model.add(constraints);
  constraints.end();
}

void OPS_cplex_solver1::addDeltaPlusConstraints(IloRangeArray &constraints) {
  for (auto k = 0; k < I_->getM(); ++k) {
    for (auto node_idx = 0; node_idx < I_->getN() - 1; ++node_idx) {
      const auto &node_string = std::to_string(node_idx);
      const auto mki = I_->getAmountOfSuccessors(k, node_string);
      if (mki == 0) continue;

      IloExpr expression(env_);
      for (auto l = 0; l < mki; ++l) {
        expression += x_[I_->getArcId(k, node_string, l, true)];
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

void OPS_cplex_solver1::addDeltaMinusConstraints(IloRangeArray &constraints) {
  for (auto k = 0; k < I_->getM(); ++k) {
    for (auto node_idx = 1; node_idx < I_->getN(); ++node_idx) {
      const auto &node_string = std::to_string(node_idx);
      const auto mki = I_->getAmountOfPredecessors(k, node_string);
      if (mki == 0) continue;

      IloExpr expression(env_);
      for (auto l = 0; l < mki; ++l)
        expression += x_[I_->getArcId(k, node_string, l, false)];
      if (node_idx < I_->getN() - 1) expression -= y_[node_idx - 1];

      const double is_last_node = node_idx == (I_->getN() - 1) ? 1.0 : 0.0;
      constraints.add(IloRange(
        env_, is_last_node, expression, is_last_node,
        ("deltaminus_" + node_string + '_' + std::to_string(k + 1)).c_str()
      ));
      expression.end();
    }
  }
}

void OPS_cplex_solver1::addMTZConstraints(IloRangeArray &constraints) {
  const int BIG_M = std::max<int>(I_->getMaxArc(), I_->getL()) + 1;
  for (auto k = 0, x_idx = 0; k < I_->getM(); ++k) {
    const auto &graph = I_->getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &i = arc.getOriginId();
      const auto &j = arc.getDestinationId();
      IloExpr expression(env_);
      expression = BIG_M * x_[x_idx] + s_[std::stoi(i)] - s_[std::stoi(j)];

      constraints.add(IloRange(
        env_, -IloInfinity, expression,
        BIG_M - I_->getT(std::stoi(i), std::stoi(j)),
        ("MTZ_" + i + "_" + j + "_" + std::to_string(k + 1)).c_str()
      ));
      expression.end();
      x_idx++;
    }
  }
}

void OPS_cplex_solver1::addLimitConstraints(IloRangeArray &constraints) {
  IloExpr start_time_expression(env_);
  start_time_expression = s_[0];
  constraints.add(IloRange(env_, 0, start_time_expression, 0, "Limit0"));
  start_time_expression.end();

  IloExpr last_time_expression(env_);
  last_time_expression = s_[I_->getN() - 1];
  constraints.add(
    IloRange(env_, -IloInfinity, last_time_expression, I_->getL(), "Limit")
  );
  last_time_expression.end();
}

}  // namespace emir