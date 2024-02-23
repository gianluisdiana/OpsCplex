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
    IloModel model(env_);
    makeModel(model);

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

void OPS_cplex_solver1::makeModel(IloModel &model) {
  const int n = I_->getN();  // Number of vertices plus two depots
  const int K = I_->getM();  // Number of vehicles
  const int L = I_->getL();  // Limit

  const int max_arc = I_->getMaxArc() + 1;
  const int big_m = max_arc > (L + 1) ? max_arc : (L + 1);

  char aux[80];

  int m = 0;

  for (int k = 0; k < K; k++) m += I_->getAmountOfSuccessors(k);

  // Variables

  // n - 2 variables y

  for (int j = 1; j < n - 1; j++) {
    sprintf(aux, "y_%d", j);
    y_.add(IloNumVar(env_, 0, 1, IloNumVar::Bool, aux));
  }

  model.add(y_);

  // n - 1 variables s
  // n - 1 variables s

  for (int j = 0; j < n; j++) {
    sprintf(aux, "s_%d", j);
    s_.add(IloNumVar(env_, 0, IloInfinity, IloNumVar::Float, aux));
  }

  model.add(s_);

  // Variables x

  for (int k = 0; k < K; k++) {
    const auto graph = I_->getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto origin_node = std::stoi(arc.getOriginId());
      const auto destination_node = std::stoi(arc.getDestinationId());
      sprintf(aux, "x_%d_%d_%d", k + 1, origin_node, destination_node);
      x_.add(IloNumVar(env_, 0, 1, IloNumVar::Bool, aux));
    }
  }

  model.add(x_);

  // Objective

  IloExpr obj(env_);

  for (int i = 1; i < n - 1; i++) obj += I_->getB(i) * y_[i - 1];

  model.add(IloMaximize(env_, obj));

  obj.end();

  // Constraints

  IloRangeArray constraints(env_);

  for (int k = 0; k < K; k++) {
    const auto mki = I_->getAmountOfPredecessors(k, std::to_string(n - 1));

    if (mki == 0) continue;
    IloExpr cut(env_);

    for (int l = 0; l < mki; l++)
      cut += x_[I_->getArcId(k, std::to_string(n - 1), l, false)];
    sprintf(aux, "deltaminus_%d_%d", n - 1, k + 1);
    constraints.add(IloRange(env_, 1.0, cut, 1.0, aux));
    cut.end();
  }

  // DELTA PLUS

  for (auto k = 0; k < K; ++k) {
    for (auto node_idx = 0; node_idx < n - 1; ++node_idx) {
      const auto mki = I_->getAmountOfSuccessors(k, std::to_string(node_idx));
      if (mki == 0) continue;

      IloExpr cut(env_);
      for (auto l = 0; l < mki; ++l)
        cut += x_[I_->getArcId(k, std::to_string(node_idx), l, true)];
      if (node_idx > 0) cut -= y_[node_idx - 1];

      sprintf(aux, "deltaplus_%d_%d", node_idx, k + 1);
      const double is_root_node = node_idx == 0 ? 1.0 : 0.0;
      constraints.add(IloRange(env_, is_root_node, cut, is_root_node, aux));
      cut.end();
    }
  }

  // DELTA MINUS

  for (int k = 0; k < K; k++) {
    for (int i = 1; i < n - 1; i++) {
      const int mki = I_->getAmountOfPredecessors(k, std::to_string(i));

      if (mki == 0) continue;

      IloExpr cut(env_);
      for (int l = 0; l < mki; l++)
        cut += x_[I_->getArcId(k, std::to_string(i), l, false)];
      cut -= y_[i - 1];
      sprintf(aux, "deltaminus_%d_%d", i, k + 1);
      constraints.add(IloRange(env_, 0.0, cut, 0.0, aux));
      cut.end();
    }
  }

  // MTZ

  int l = 0;
  for (auto k = 0; k < K; ++k) {
    const auto &graph = I_->getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const int i = std::stoi(arc.getOriginId());
      const int j = std::stoi(arc.getDestinationId());
      IloExpr cut(env_);
      cut = big_m * x_[l] + s_[i] - s_[j];

      sprintf(aux, "MTZ_%d_%d_%d", i, j, k + 1);
      constraints.add(
        IloRange(env_, -IloInfinity, cut, big_m - I_->getT(i, j), aux)
      );
      cut.end();
      l++;
    }
  }

  // LIMIT

  {
    IloExpr cut(env_);

    cut = s_[0];

    sprintf(aux, "Limit0");
    constraints.add(IloRange(env_, 0, cut, 0, aux));
    cut.end();
  }

  {
    IloExpr cut(env_);

    cut = s_[n - 1];

    sprintf(aux, "Limit");
    constraints.add(IloRange(env_, -IloInfinity, cut, L, aux));
    cut.end();
  }

  model.add(constraints);
  constraints.end();
}

}  // namespace emir