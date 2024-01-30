#include <set>

#include <OPS_input_t.hpp>
#include <matrix.hpp>

namespace emir {

/*OPS_input_t::OPS_input_t(const OpsInstance& instance, const
std::vector<int>& nJ, const std::vector<int>& nK): instance_(instance), succ_(),
    pred_(),
    succ_inx_(),
    pred_inx_(),
    A_succ_(),
    inv_succ_(),
    t_cost_()
{
    const int K = nK.size();
    const int n = nJ.size();

    nodes_k_.resize(K);

    succ_.resize(K,n);
    pred_.resize(K,n);

    succ_inx_.resize(K,n);
    pred_inx_.resize(K,n);

    inv_succ_.resize(K);

    arcs_k_.resize(K);

    t_cost_.resize(n,n);

    int l = 0;

    for (int ki = 0; ki < K; ki++) {

        const int k = nK[ki];

        const std::vector<int>& Jk = instance.getJk(k);
        const int nJ = Jk.size();

        matrix<int>& A_inv  = inv_succ_[ki];
        A_inv.resize(n,n);
        A_inv.init(-1);

        std::vector<int>& arcs_k = arcs_k_[ki];

        std::vector<int> nodes(n,0);

        for (auto j: Jk) {

            succ_(ki + 1, 1).push_back(j);
            succ_inx_(ki + 1, 1).push_back(l);
            A_succ_.push_back(get_pos(k, 0, j));
            A_inv(0 + 1,j + 1) = l;
            arcs_k.push_back(l);

            l++;

            nodes[0]++;
            nodes[j]++;
        }



        succ_(k + 1, 1).push_back(n - 1);
        succ_inx_(k + 1, 1).push_back(l);
        A_succ_.push_back(get_pos(k, 0, n - 1));
        A_inv(0 + 1, n) = l;
        arcs_k.push_back(l);

        l++;

        nodes[0]++;
        nodes[n - 1]++;

        for (int i = 0; i < nJ; i++) {

            const int Ji = Jk[i];

            for (int j = 0; j < nJ; j++)
                if (i != j) {

                    const int Jj = Jk[j];

                    succ_(k + 1,Ji + 1).push_back(Jj);
                    succ_inx_(k + 1,Ji + 1).push_back(l);
                    A_succ_.push_back(get_pos(k, Ji, Jj));

                    A_inv(Ji + 1,Jj + 1) = l;

                    arcs_k.push_back(l);

                    l++;

                    nodes[Ji]++;
                    nodes[Jj]++;
                }

            succ_(k + 1,Ji + 1).push_back(n - 1);
            succ_inx_(k + 1,Ji + 1).push_back(l);
            A_succ_.push_back(get_pos(k, Ji, n - 1));

            A_inv(Ji + 1, n) = l;

            arcs_k.push_back(l);

            l++;

            nodes[Ji]++;
            nodes[n - 1]++;
        }

        std::vector<int>& nodes_k = nodes_k_[k];
        for (int i = 1; i < n - 1; i++)
            if (nodes[i] > 0)
                nodes_k.push_back(i);
    }

} */

void OpsInput::resizeGraphStructures() {
  const int m = getM();
  const int n = getN();

  nodes_k_.resize(m);
  succ_.resize(m, n);
  pred_.resize(m, n);
  succ_inx_.resize(m, n);
  pred_inx_.resize(m, n);
  inv_succ_.resize(m);
  arcs_k_.resize(m);
  t_cost_.resize(n, n);
}

void OpsInput::updateGraphStructures(
  int k, int Ji, int Jj, int &l, GOMA::matrix<int> &A_inv,
  std::vector<int> &arcs_k, std::vector<int> &nodes
) {
  // std::cout <<"Arco: " << Ji << ", " << Jj << '\n';

  succ_(k + 1, Ji + 1).push_back(Jj);
  succ_inx_(k + 1, Ji + 1).push_back(l);
  A_succ_.push_back(get_pos(k, Ji, Jj));

  A_inv(Ji + 1, Jj + 1) = l;

  arcs_k.push_back(l);

  nodes[Ji]++;
  nodes[Jj]++;

  l++;
}

void OpsInput::addPredecessor(int k, int Ji, int Jj, int &l) {
  pred_(k + 1, Jj + 1).push_back(Ji);
  pred_inx_(k + 1, Jj + 1).push_back(l);

  l++;
}

void OpsInput::processTMatrix() {
  const int n = getN();

  for (int i = 1; i <= n - 1; i++) {
    // shortest_path(aux, i - 1, dist, prev);
    for (int j = 2; j <= n; j++)
      if (i != j) {
        // std::cout << dist[j - 1] - instance.get_T()(i,j) << '\n';
        // assert(dist[j - 1] <= instance.get_T()(i,j));

        t_cost_(i, j) = get_T(i, j);
      } else
        t_cost_(i, j) = OpsInstance::kInfiniteTime;
  }

  t_cost_(1, n) = 0;

  for (int j = 1; j <= n; j++) t_cost_(n, j) = OpsInstance::kInfiniteTime;

  for (int i = 1; i <= n; i++) t_cost_(i, 1) = OpsInstance::kInfiniteTime;

  // t_cost_.write_raw(std::cout);
}

void OpsInput::makeGraphArcs()  // AQUÍ!!!!!!!!!
{
  int l = 0;

  const int n = getN();
  const int m = getM();

  for (int k = 0; k < m; k++) {
    const std::vector<int> &Jk = getJk(k);

    const int nJ = Jk.size();

    GOMA::matrix<int> &A_inv = inv_succ_[k];
    A_inv.resize(n, n);
    A_inv.init(-1);

    std::vector<int> &arcs_k = arcs_k_[k];

    std::vector<int> nodes(n, 0);

    for (auto j : Jk) {
      if (t_cost_(1, j + 1) < OpsInstance::kInfiniteTime)
        updateGraphStructures(k, 0, j, l, A_inv, arcs_k, nodes);
    }

    updateGraphStructures(k, 0, n - 1, l, A_inv, arcs_k, nodes);

    for (int i = 0; i < nJ; i++) {
      const int Ji = Jk[i];

      for (int j = 0; j < nJ; j++)
        if (i != j) {
          const int Jj = Jk[j];

          if (t_cost_(Ji + 1, Jj + 1) < OpsInstance::kInfiniteTime)
            updateGraphStructures(k, Ji, Jj, l, A_inv, arcs_k, nodes);
        }

      updateGraphStructures(k, Ji, n - 1, l, A_inv, arcs_k, nodes);
    }

    // std::cout << "Concluido" << '\n';

    std::vector<int> &nodes_k = nodes_k_[k];
    for (int i = 1; i < n - 1; i++)
      if (nodes[i] > 0) nodes_k.push_back(i);
  }
}

void OpsInput::makePredecessors() {
  const int m = getM();
  const int n = getN();

  int l = 0;

  for (int k = 0; k < m; k++) {
    const std::vector<int> &Jk = getJk(k);
    const int nJ = Jk.size();

    for (auto j : Jk) addPredecessor(k, 0, j, l);

    addPredecessor(k, 0, n - 1, l);

    for (int i = 0; i < nJ; i++) {
      const int Ji = Jk[i];

      for (int j = 0; j < nJ; j++)
        if (i != j) {
          const int Jj = Jk[j];

          addPredecessor(k, Ji, Jj, l);
        }

      addPredecessor(k, Ji, n - 1, l);
    }
  }
}

void OpsInput::build() {
  resizeGraphStructures();

  makeGraphArcs();

  makePredecessors();

  processTMatrix();

#ifndef NDEBUG
  test_succ();
  test_pred();
  test_A_succ();
#endif
}

OpsInput::OpsInput(bool build) :
  OpsInstance(), succ_(), pred_(), succ_inx_(), pred_inx_(), A_succ_(),
  inv_succ_(), t_cost_() {
  if (build) this->build();
}

int OpsInput::get_max_arc() const {
  int max = 0;

  const int sz = get_A_succ_sz();

  for (int l = 0; l < sz; l++) {
    const int pos = get_A_succ(l);

    int i, j, k;
    get_pos(pos, k, i, j);

    if (max < t_cost_(i + 1, j + 1)) max = t_cost_(i + 1, j + 1);
  }

  return max;
}

int OpsInput::get_max_nodes() const {
  int m = -1;
  const int K = getM();

  for (int k = 0; k < K; k++) {
    const int csz = nodes_k_[k].size();

    m = m < csz ? csz : m;
  }

  return m;
}

void OpsInput::get_sync_stat(
  int &nsync, int &maxgsync, int &mingsync, double &avggsync
) const {
  const int K = getM();
  const int n = getN();

  std::vector<int> V(n);

  for (int &v : V) v = 0;

  for (int k = 0; k < K; k++) {
    const std::vector<int> &nk = nodes_k_[k];

    for (auto v : nk) V[v]++;
  }

  nsync = 0;
  maxgsync = 1;
  mingsync = n;
  avggsync = 0;

  int cnt = 0;

  for (int i = 1; i < n - 1; i++) {
    assert(V[i] > 0);

    if (V[i] > 1) {
      cnt++;

      nsync++;

      if (V[i] > maxgsync) maxgsync = V[i];

      if (V[i] < mingsync) mingsync = V[i];

      avggsync += V[i];
    }
  }

  if (cnt > 0)
    avggsync /= (double)(cnt);
  else {
    maxgsync = 0;
    mingsync = 0;
    avggsync = 0;
  }
}

double OpsInput::get_avg_nodes() const {
  double m = 0;
  const int K = getM();

  for (int k = 0; k < K; k++) {
    const int csz = nodes_k_[k].size();
    m += csz;
  }

  return m / (double)(K);
}

OpsInput::~OpsInput() {}

int OpsInput::get_msucc(int k) const {
  int m = 0;

  for (int i = 0; i < getN(); i++) m += get_nsucc(k, i);

  return m;
}

void OpsInput::get_pos(int pos, int &k, int &i, int &j) const {
  const int n = getN();
  const int m = n * n;

  k = pos / m;
  int res = pos % m;

  i = res / n;
  j = res % n;
}

void OpsInput::get_path(
  const std::vector<int> &v, int k, std::vector<int> &arcs,
  std::vector<bool> &visited
) const {
  const int sz = v.size() - 1;
  assert(sz > 0);

  for (int l = 0; l < sz; l++) {
    visited[v[l]] = true;
    visited[v[l + 1]] = true;

    const int p = get_inv_succ(k, v[l], v[l + 1]);
    arcs.push_back(p);
  }
}

#ifndef NDEBUG

void OpsInput::test_succ() {
  const int K = getM();

  for (int k = 0; k < K; k++) {
    std::set<int> SJk;
    const std::vector<int> &Jk = getJk(k);
    const int nJ = Jk.size();

    // std::cout << "Grupo: " << k << '\n';

    // std::cout << "     ";

    for (auto i : Jk) {
      SJk.insert(i);

      // std::cout << std::setw(3) << i << " ";
    }
    // std::cout << '\n';

    for (int i = 0; i < nJ; i++) {
      const int Ji = Jk[i];
      const std::vector<int> &v = succ_(k + 1, Ji + 1);
      const int sz = v.size();

      // std::cout << "    Sucesores de : " << Ji << '\n';

      // for (int j: v)
      //	std::cout << std::setw(3) << j << " ";
      // std::cout << '\n';

      for (int l = 0; l < sz - 1; l++) {
        const int j = v[l];

        // std::cout << j << "      Checking: ";

        // if (SJk.find(j)== SJk.end())
        // exit(1);

        assert(SJk.find(j) != SJk.end());

        // std::cout << "OK" << '\n';
      }
    }
  }
}

void OpsInput::test_pred() {
  const int K = getM();

  for (int k = 0; k < K; k++) {
    std::set<int> SJk;
    const std::vector<int> &Jk = getJk(k);
    const int nJ = Jk.size();

    for (auto i : Jk) SJk.insert(i);

    for (int i = 0; i < nJ; i++) {
      const int Ji = Jk[i];
      std::vector<int> &v = pred_(k + 1, i + 1);
      const int sz = v.size();

      for (int l = 1; l < sz; l++) {
        const int j = v[l];
        assert((SJk.find(Ji) != SJk.end()) && (SJk.find(j) != SJk.end()));
      }
    }
  }
}

void OpsInput::test_A_succ() {
  const int sz = A_succ_.size();
  const int n = getN();

  for (int l = 0; l < sz; l++) {
    const int pos = A_succ_[l];

    int i, j, k;
    get_pos(pos, k, i, j);

    if ((i != 0) && (j != n - 1)) {
      std::set<int> SJk;
      const std::vector<int> &Jk = getJk(k);

      for (auto Ji : Jk) SJk.insert(Ji);

      assert((SJk.find(i) != SJk.end()) && (SJk.find(j) != SJk.end()));
    }
  }
}

#endif

void OpsInput::writeStatisticsHdr(std::ostream &os) const {
  OpsInstance::writeStatisticsHdr(os);

  os << "$n$"
     << "\t";
  os << "$m$"
     << "\t";

  os << "$\\max|J_k|$"
     << "\t";
  os << "$\\bar{|J_k|}$"
     << "\t";

  os << "nsync"
     << "\t";
  os << "$\\max\\partial\\mbox{sync}$"
     << "\t";
  os << "$\\bar{\\partial\\mbox{sync}$"
     << "\t";

  os << "$L$"
     << "\t";
}

void OpsInput::writeStatistics(std::ostream &os) const {
  OpsInstance::writeStatistics(os);

  int nsync, maxgsync, mingsync;
  double avggsync;

  get_sync_stat(nsync, maxgsync, mingsync, avggsync);

  os << std::setw(4) << getN() << "\t";
  os << std::setw(4) << getM() << "\t";

  os << std::setw(4) << get_max_nodes() << "\t";
  os << std::setw(6) << std::fixed << std::setprecision(1) << get_avg_nodes()
     << "\t";

  os << std::setw(4) << nsync << "\t";
  os << std::setw(4) << maxgsync << "\t";
  os << std::setw(6) << std::fixed << std::setprecision(1) << avggsync << "\t";

  os << std::setw(6) << getL() << "\t";
  os << "\t";
}

// ------------------------------- Operators ------------------------------- //

std::istream &operator>>(std::istream &is, OpsInput &ops_input) {
  is >> static_cast<OpsInstance &>(ops_input);
  ops_input.build();
  return is;
}

}  // namespace emir
