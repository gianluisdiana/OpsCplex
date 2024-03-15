#include <graph.hpp>

namespace emir {

Graph::Graph() : arcs_(), nodes_() {}

const std::vector<std::string> Graph::getNodesId() const {
  std::vector<std::string> nodes_id;
  for (const auto &[id, _] : nodes_) nodes_id.push_back(id);
  return nodes_id;
}

// -------------------------------- Adders -------------------------------- //

void Graph::addArc(
  const std::string &from_id, const std::string &to_id, const int cost
) {
  const auto& from_node = searchNode(from_id);
  const auto& to_node = searchNode(to_id);
  arcs_.emplace_back(from_node, to_node, cost);
}

// ---------------------------- Private Methods ---------------------------- //

const std::shared_ptr<Node> &Graph::searchNode(const std::string &id) {
  const auto& it = nodes_.find(id);
  if (it != nodes_.end()) return it->second;
  nodes_[id] = std::make_shared<Node>(id);
  return nodes_[id];
}

}  // namespace emir