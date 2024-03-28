#include <graph.hpp>

namespace emir {

Graph::Graph() = default;

std::vector<std::string> Graph::getNodesId() const {
  std::vector<std::string> nodes_id;
  for (const auto &[id, _] : nodes_) { nodes_id.push_back(id); }
  return nodes_id;
}

// -------------------------------- Adders -------------------------------- //

void Graph::addArc(
  const std::string &from_id, const std::string &to_id, const int cost
) {
  const auto &from_node = searchNode(from_id);
  const auto &to_node = searchNode(to_id);
  arcs_.emplace_back(from_node, cost, to_node);
}

// ---------------------------- Private Methods ---------------------------- //

const std::shared_ptr<Node> &Graph::searchNode(const std::string &node_id) {
  if (nodes_.find(node_id) != nodes_.end()) { return nodes_.at(node_id); }
  nodes_[node_id] = std::make_shared<Node>(node_id);
  return nodes_[node_id];
}

}  // namespace emir