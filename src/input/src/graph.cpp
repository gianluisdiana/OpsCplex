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