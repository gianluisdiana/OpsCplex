// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file OPS_input_t.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.2.0
 * @date January 21, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the input instance for the
 *  O.P.S. problem, representing the math model with a graph for each sliding bar.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <ops_input.hpp>

namespace emir {

OpsInput::OpsInput() : OpsInstance(), graphs_(getM()) {}

// -------------------------------- Getters -------------------------------- //

std::size_t OpsInput::getAmountOfSuccessors(
  const int sliding_bar_index, const std::string &node_id
) const {
  if (!graphs_[sliding_bar_index].hasNode(node_id)) return 0;
  return graphs_[sliding_bar_index].getAmountOfSuccessors(node_id);
}

std::size_t OpsInput::getAmountOfPredecessors(
  const int sliding_bar_index, const std::string &node_id
) const {
  if (!graphs_[sliding_bar_index].hasNode(node_id)) return 0;
  return graphs_[sliding_bar_index].getAmountOfPredecessors(node_id);
}

unsigned int OpsInput::getMaxArc() const {
  unsigned int max_arc = 0;
  for (const auto &graph : graphs_) {
    for (const auto &arc : graph.getArcs()) {
      if (arc.getCost() > max_arc) max_arc = arc.getCost();
    }
  }
  return max_arc;
}

// ----------------------------- Stats Getters ----------------------------- //

const std::string OpsInput::getStatistics() const {
  auto bars_stats = getBarsStats();
  auto nodes_stats = getNodesStats();
  std::stringstream ss;

  // ss << OpsInstance::getStatistics()
  ss << std::setw(4) << getN() << '\t' << std::setw(4) << getM() << '\t'
     << std::setw(4) << nodes_stats.max_amount << '\t' << std::setw(6)
     << std::fixed << std::setprecision(1) << nodes_stats.getAvgAmount() << '\t'
     << std::setw(4) << bars_stats.amount << '\t' << std::setw(4)
     << bars_stats.max_amount << '\t' << std::setw(6) << std::fixed
     << std::setprecision(1) << bars_stats.getAvgAmount() << '\t'
     << std::setw(6) << getL() << "\t\t";
  return ss.str();
}

// ------------------------------ Operators ------------------------------ //

std::istream &operator>>(std::istream &is, OpsInput &ops_input) {
  is >> static_cast<OpsInstance &>(ops_input);
  ops_input.truncateT();
  ops_input.createGraphArcs();
  return is;
}

// --------------------------- Private Methods --------------------------- //

void OpsInput::truncateT() {
  const auto n = getN();
  for (auto i = 1; i <= n; ++i) {
    setTOutOfRange(i, 1);
    setTOutOfRange(n, i);
    setTOutOfRange(i, i);
  }
  setTZero(1, n);
}

void OpsInput::createGraphArcs() {
  const auto n = getN();
  const auto m = getM();
  for (auto k = 0; k < m; ++k) {
    auto graph = Graph();
    graph.addArc(0, n - 1, getT(0, n - 1));
    const auto &Jk = getJk(k);
    for (const auto &Ji : Jk) {
      graph.addArc(Ji, n - 1, getT(Ji, n - 1));
      if (getT(0, Ji) < OpsInstance::kInfiniteTime) {
        graph.addArc(0, Ji, getT(0, Ji));
      }
      for (const auto &Jj : Jk) {
        if (getT(Ji, Jj) < OpsInstance::kInfiniteTime) {
          graph.addArc(Ji, Jj, getT(Ji, Jj));
        }
      }
    }
    graphs_.push_back(graph);
  }
}

// ----------------------- Private Statistics Getters ----------------------- //

const Stats OpsInput::getBarsStats() const {
  const int n = getN();
  std::vector<unsigned int> bars_needed_per_object(n, 0);
  Stats stats(getM());

  for (const auto &graph : graphs_) {
    for (const auto &id : graph.getNodesId())
      bars_needed_per_object[std::stoi(id)]++;
  }

  for (auto i = 1; i < getN() - 1; i++) {
    auto bars_needed = bars_needed_per_object[i];
    if (bars_needed == 1) continue;
    stats.updateMax(bars_needed);
    stats.updateMin(bars_needed);
    stats.amount_sum += bars_needed;
    stats.amount++;
  }

  return stats;
}

const Stats OpsInput::getNodesStats() const {
  Stats stats(getN());
  for (const auto &graph : graphs_) {
    auto amount_of_nodes = graph.getAmountOfNodes();
    stats.updateMax(amount_of_nodes);
    stats.updateMin(amount_of_nodes);
    stats.amount_sum += amount_of_nodes;
  }
  stats.amount = getM();
  return stats;
}

}  // namespace emir