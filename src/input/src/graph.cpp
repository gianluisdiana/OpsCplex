// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file graph.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the digraph.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <memory>
#include <ranges>
#include <vector>

#include <graph.hpp>
#include <node.hpp>

namespace emir {

// Change to use std::view::keys
std::vector<unsigned int> Graph::getNodesId() const {
  std::vector<unsigned int> nodes_id(nodes_.size());
  for (const auto &[idx, entry] : std::views::enumerate(nodes_)) {
    nodes_id[idx] = entry.first;
  }
  return nodes_id;
}

// -------------------------------- Adders -------------------------------- //

void Graph::addArc(
  const unsigned int from_id, const unsigned int to_id, const int cost
) {
  const auto &from_node = searchNode(from_id);
  const auto &to_node = searchNode(to_id);
  arcs_.emplace_back(from_node, cost, to_node);
}

// ---------------------------- Private Methods ---------------------------- //

const std::shared_ptr<Node> &Graph::searchNode(const unsigned int node_id) {
  if (nodes_.find(node_id) != nodes_.end()) { return nodes_.at(node_id); }
  nodes_[node_id] = std::make_shared<Node>(node_id);
  return nodes_[node_id];
}

}  // namespace emir