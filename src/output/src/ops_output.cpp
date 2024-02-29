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
  os << std::setw(4) << n_customers() << "\t";

  if (found_)
    os << std::setw(10) << get_obj() << "\t";
  else
    os << std::setw(10) << 99999999 << "\t";

  if (found_)
    os << std::setw(10) << std::fixed << std::setprecision(1) << length() * 10
       << "\t";
  else
    os << std::setw(10) << 0 << "\t";

  if (found_)
    os << std::setw(4) << 1 << "\t";
  else
    os << std::setw(4) << 0 << "\t";

  if (optimal_)
    os << std::setw(4) << 1 << "\t";
  else
    os << std::setw(4) << 0 << "\t";
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

int OpsOutput::get_obj() const {
  assert((s_.size() > 0) && (s_[0] != -1));

  int obj = 0;

  for (int j = y_.size() - 1; j >= 0; j--)
    if (y_[j] > 0) {
      const double b = input_.OpsInput::getB(j);
      obj += y_[j] * b;
    }

  return obj;
}

int OpsOutput::n_customers() const {
  int cnt = 0;
  const int nc = y_.size() - 1;

  for (int i = 1; i < nc; i++)
    if (y_[i] > 0) cnt++;

  return cnt;
}

bool OpsOutput::check() {
  found_ = true;

  const int n = input_.getN();
  const int K = input_.getM();
  const int L = input_.getL();

  std::vector<int> i_degree(n);
  std::vector<int> o_degree(n);

  for (int i = 0; i < n; i++) {
    i_degree[i] = 0;
    o_degree[i] = 0;
  }

  for (int k = 0; k < K; k++)
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++) {
        if (get_x(k, i, j) == 1) {
          i_degree[j]++;
          o_degree[i]++;
        }
      }

  assert(o_degree[0] == K);
  assert(i_degree[n - 1] == K);

  for (int i = 1; i < n - 1; i++) assert((o_degree[i] - i_degree[i]) == 0);

  for (int i = 1; i < n - 1; i++)
    if (i_degree[i] > 0)
      assert(y_[i] == 1);
    else
      assert(y_[i] == 0);

  const double rL = (double)(L) / input_.getScalingFactor();

  /*for (int j = 0; j < n; j++)
                  std::cout << "Nodo: " << std::setw(3) << j << ": " <<
  std::setw(8) << std::fixed << std::setprecision(1) << s_[j]
                       << " -> " << std::setw(8) << std::fixed <<
  std::setprecision(1) << rL << '\n'; std::cout << '\n'; */

  for (int i = 0; i < n; i++) {
    if (s_[i] > rL + OpsOutput::kMaxTimeMargin) {
      found_ = false;

      std::cout << "Nodo: " << i << ": " << s_[i] << " -> " << rL << '\n';

      for (int j = 0; j < n; j++)
        std::cout << "Nodo: " << std::setw(3) << j << ": " << std::setw(8)
                  << std::fixed << std::setprecision(1) << s_[j] << " -> "
                  << std::setw(8) << std::fixed << std::setprecision(1) << rL
                  << '\n';
      std::cout << '\n';

      assert(s_[i] <= rL + OpsOutput::kMaxTimeMargin);
      exit(1);
    }
  }

  /*std::cout << '\n';

  for (int j = 0; j < n; j ++)
     std::cout << "Nodo: "<< std::setw(3)<< j << ": " << std::setw(8) <<
  std::fixed << std::setprecision(1) <<  s_[j] << " -> " << std::setw(8) <<
  std::fixed << std::setprecision(1) << rL << '\n'; std::cout << '\n';     */

  return found_;
}

// ------------------------------- Operators ------------------------------- //

std::ostream &operator<<(std::ostream &os, const OpsOutput &output) {
  json x_json, t_cost;
  output.x_.get_json(x_json);
  const json output_json = {
    {"x", x_json},
    {"y", output.y_},
    {"s", output.s_},
    {"h", output.h_},
    {"optimal", output.optimal_}};
  return os << output_json.dump(2);
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
  for (int i = 1; i < n - 1; ++i) {
    const int value = y[i - 1];
    assert(value == 1 || value == 0);
    y_[i] = value;
  }
}

void OpsOutput::setS(const std::vector<double> &s) {
  const int n = input_.getN();
  s_[0] = 0;
  if (s.size() == 0) return;
  for (int i = 1; i < n ; i++) {
    const int val = s[i - 1];
    assert(val >= 0);
    s_[i] = val / input_.getScalingFactor();
  }
}

}  // namespace emir