#include <cassert>
#include <cmath>
#include <iomanip>
#include <set>

#include <ops_output.hpp>

namespace emir {

const double OpsOutput::kMaxTimeMargin = 1e-2;

OpsOutput::OpsOutput(const OpsInput &input) :
  input_(input), x_(input.getN() * input.getM(), input.getN()),
  y_(input.getN(), 0), s_(input.getN(), 0),
  h_(input.getN(), double(input.getL()) / input.getScalingFactor()),
  optimal_(false), found_(false) {}

OpsOutput::~OpsOutput() {}

void OpsOutput::set(
  const std::vector<double> &x, const std::vector<double> &y,
  const std::vector<double> &s, bool isOptimal
) {
  found_ = true;
  optimal_ = isOptimal;
  setX(x);
  setY(y);
  setS(s);
  check();
}

void OpsOutput::write_statistics(std::ostream &os) const {
  os << std::setw(4) << getObjectsVisited() << '\t';
  if (found_) {
    os << std::setw(10) << getTotalProfit() << '\t' << std::setw(10)
       << std::fixed << std::setprecision(1) << length() * 10 << '\t'
       << std::setw(4) << 1 << '\t';
  } else {
    os << std::setw(10) << 99'999'999 << '\t' << std::setw(10) << 0 << '\t'
       << std::setw(4) << 0 << '\t';
  }
  os << std::setw(4) << (optimal_ ? 1 : 0) << '\t';
}

std::ostream &OpsOutput::write(std::ostream &os) const {
  /* x_.write_raw(os);

  os << '\n';*/
  std::vector<int> r(input_.getN(), 0);

  const int n = input_.getN();

  /*for (int i = 0; i < n; i++)
      os << std::setw(5) << y_[i];

  os << '\n';
  os << '\n'; */

  for (int i = 0; i < n; i++)
    if (fabs(s_[i] > 0.0001))
      os << std::setw(4) << i << " " << std::setw(9) << std::fixed
         << std::setprecision(2) << s_[i] << " " << std::setw(9) << std::fixed
         << std::setprecision(2) << h_[i] << " " << '\n';

  os << '\n';
  os << '\n';

  return os;
}

// ------------------------------- Operators ------------------------------- //

std::ostream &operator<<(std::ostream &os, const OpsOutput &output) {
  return os << nlohmann::json({{"x", output.x_.toJson()},
                               {"y", output.y_},
                               {"s", output.s_},
                               {"h", output.h_},
                               {"optimal", output.optimal_}}
         ).dump(2);
}

// ------------------------------- Setters --------------------------------- //

void OpsOutput::setX(const std::vector<double> &x) {
  x_.init(0);
  for (int k = 0; k < input_.getM(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const int value = x[arc.getId()];
      assert(value == 1 || value == 0);
      if (value == 0) continue;
      const int i = std::stoi(arc.getOriginId());
      const int j = std::stoi(arc.getDestinationId());
      setXAsTrue(k, i, j);
    }
  }
}

void OpsOutput::setY(const std::vector<double> &y) {
  const int n = input_.getN();
  y_[0] = 1;
  y_[n - 1] = 1;
  if (y.size() == 0) return;
  for (int idx = 1; idx < n - 1; ++idx) {
    const int value = y[idx - 1];
    assert(value == 1 || value == 0);
    y_[idx] = value;
  }
}

void OpsOutput::setS(const std::vector<double> &s) {
  const int n = input_.getN();
  s_[0] = 0;
  if (s.size() == 0) return;
  for (int idx = 1; idx < n; ++idx) {
    const int value = s[idx - 1];
    assert(value >= 0);
    s_[idx] = value / input_.getScalingFactor();
  }
}

// ------------------------------- Getters --------------------------------- //

std::size_t OpsOutput::getObjectsVisited() const {
  std::size_t amount = 0;
  for (std::size_t idx = 1; idx < y_.size() - 1; ++idx) {
    if (y_[idx] > 0) ++amount;
  }
  return amount;
}

int OpsOutput::getTotalProfit() const {
  assert(s_.size() > 0 && s_[0] != -1);
  int solutionValue = 0;
  for (std::size_t idx = 0; idx < y_.size(); ++idx) {
    if (y_[idx] > 0) solutionValue += y_[idx] * input_.getB(idx);
  }
  return solutionValue;
}

// ---------------------------- Utility Methods ---------------------------- //

void OpsOutput::check() const {
  checkArcs();
  checkTime();
}

void OpsOutput::checkArcs() const {
  const auto n = input_.getN();
  const auto K = input_.getM();
  std::vector<int> amount_of_arrives(n, 0);
  std::vector<int> amount_of_departures(n, 0);

  for (int k = 0; k < K; k++) {
    for (auto origin_node = 0; origin_node < n; ++origin_node) {
      for (auto destiny_node = 0; destiny_node < n; ++destiny_node) {
        if (getX(k, origin_node, destiny_node) == 1) {
          amount_of_arrives[destiny_node]++;
          amount_of_departures[origin_node]++;
        }
      }
    }
  }

  // The first node has to be used in each sliding bar, same with the last node
  assert(amount_of_departures[0] == K && amount_of_arrives[n - 1] == K);

  for (std::size_t idx = 1; idx < n - 1; ++idx) {
    // A node must have the same number of arrival and departure arcs
    assert(amount_of_departures[idx] == amount_of_arrives[idx]);
    // The node must be visited in order to have arrival / departure arcs
    assert(y_[idx] == (amount_of_arrives[idx] > 0 ? 1 : 0));
  }
}

void OpsOutput::checkTime() const {
  const double rL = double(input_.getL()) / input_.getScalingFactor();
  for (const auto &s : s_) {
    if (s > rL + OpsOutput::kMaxTimeMargin) {
      assert(s <= rL + OpsOutput::kMaxTimeMargin);
      exit(1);
    }
  }
}

}  // namespace emir