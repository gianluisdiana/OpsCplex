#include <cassert>
#include <cmath>
#include <iomanip>
#include <set>

#include <OPS_output_t.hpp>

namespace emir {

const double OPS_output_t::kMaxTimeMargin = 1.0e-2;

OPS_output_t::OPS_output_t(const OpsInput &I) :
  I_(I), x_(I_.getN() * I_.getM(), I_.getN()), y_(I_.getN()), s_(I_.getN()),
  h_(I_.getN()), t_cost_(I_.getN(), I_.getN()), optimal_(false), found_(false) {
  x_.init(0);

  const int n = I_.getN();
  const int K = I_.getM();

  for (int k = 0; k < K; k++) x_(1 + k * n, n) = 1;

  const int sz_y = y_.size();

  for (int i = 0; i < sz_y; i++) y_[i] = 0;

  const int sz_s = s_.size();

  for (int i = 0; i < sz_s; i++) s_[i] = 0;

  const int sz_h = h_.size();

  const int L = I.getL();
  const double scal_factor = I.getScalingFactor();

  for (int i = 0; i < sz_h; i++) h_[i] = L / scal_factor;

  init_t_cost();

  check();
}

OPS_output_t::~OPS_output_t() {}

void OPS_output_t::init_t_cost() {
  const int n = I_.getN();

  for (int i = 1; i <= n - 1; i++) {
    for (int j = 2; j <= n; j++)
      if ((i != j) && !((i == 1) && (j == n))) {
        t_cost_(i, j) = I_.getT(i - 1, j - 1);
      } else
        t_cost_(i, j) = OpsInstance::kInfiniteTime;
  }

  t_cost_(1, n) = 0;

  for (int j = 1; j <= n - 1; j++) t_cost_(n, j) = OpsInstance::kInfiniteTime;

  for (int i = 1; i <= n; i++) t_cost_(i, 1) = OpsInstance::kInfiniteTime;
}

bool OPS_output_t::set(
  const std::vector<double> &x, const std::vector<double> &y,
  const std::vector<double> &s, bool optimal
) {
  found_ = true;
  optimal_ = optimal;

  x_.init(0);

  for (int k = 0; k < I_.getM(); k++) {
    auto graph = I_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const int i = std::stoi(arc.getOriginId());
      const int j = std::stoi(arc.getDestinationId());
      const int value = round(x[I_.calculateXIndex(k, i, j)]);

      assert(value <= 1);
      assert(value >= 0);

      set_x(k, i, j) = 1;
    }
  }

  // const int x_sz = x.size();

  // for (int l = 0; l < x_sz; l++) {
  //   const int arc = I_.get_A_succ(l);

  //   int i;
  //   int j;
  //   int k;

  //   I_.get_pos(arc, k, i, j);

  //   const int val = round(x[l]);

  //   assert(val <= 1);
  //   assert(val >= 0);

  //   if (val == 1) set_x(k, i, j) = 1;
  // }
  const int n = I_.getN();

  y_[0] = 1;
  y_[n - 1] = 1;

  if (y.size() > 0) {
    for (int i = 1; i < n - 1; i++) {
      const int val = round(y[i - 1]);

      assert(val <= 1);
      assert(val >= 0);

      y_[i] = val;
    }
  }

  s_[0] = 0;

  if (s.size() > 0) {
    for (int i = 1; i < n; i++) {
      const int val = s[i - 1];

      assert(val >= 0);

      s_[i] = val;
    }
  }

  for (int i = 0; i < n; i++) s_[i] /= I_.getScalingFactor();

  // x_.write_raw(std::cout);

  check();

  return found_;
}

void OPS_output_t::write_statistics(std::ostream &os) const {
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

std::ostream &OPS_output_t::write(std::ostream &os) const {
  /* x_.write_raw(os);

  os << '\n';*/
  std::vector<int> r(I_.getN(), 0);

  const int n = I_.getN();

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

int OPS_output_t::get_x(const int k, const int i, const int j) const {
  return x_(k * I_.getN() + i + 1, j + 1);
}

int &OPS_output_t::set_x(const int k, const int i, const int j) {
  return x_(k * I_.getN() + i + 1, j + 1);
}

int OPS_output_t::get_obj() const {
  assert((s_.size() > 0) && (s_[0] != -1));

  int obj = 0;

  for (int j = y_.size() - 1; j >= 0; j--)
    if (y_[j] > 0) {
      const double b = I_.OpsInput::getB(j);
      obj += y_[j] * b;
    }

  return obj;
}

int OPS_output_t::n_customers() const {
  int cnt = 0;
  const int nc = y_.size() - 1;

  for (int i = 1; i < nc; i++)
    if (y_[i] > 0) cnt++;

  return cnt;
}

double OPS_output_t::length() const {
  return s_[y_.size() - 1];
}

bool OPS_output_t::check() {
  found_ = true;

  const int n = I_.getN();
  const int K = I_.getM();
  const int L = I_.getL();

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

  const double rL = (double)(L) / I_.getScalingFactor();

  /*for (int j = 0; j < n; j++)
                  std::cout << "Nodo: " << std::setw(3) << j << ": " <<
  std::setw(8) << std::fixed << std::setprecision(1) << s_[j]
                       << " -> " << std::setw(8) << std::fixed <<
  std::setprecision(1) << rL << '\n'; std::cout << '\n'; */

  for (int i = 0; i < n; i++) {
    if (s_[i] > rL + OPS_output_t::kMaxTimeMargin) {
      found_ = false;

      std::cout << "Nodo: " << i << ": " << s_[i] << " -> " << rL << '\n';

      for (int j = 0; j < n; j++)
        std::cout << "Nodo: " << std::setw(3) << j << ": " << std::setw(8)
                  << std::fixed << std::setprecision(1) << s_[j] << " -> "
                  << std::setw(8) << std::fixed << std::setprecision(1) << rL
                  << '\n';
      std::cout << '\n';

      assert(s_[i] <= rL + OPS_output_t::kMaxTimeMargin);
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
}  // namespace emir
