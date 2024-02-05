#include <graph.hpp>

namespace emir {

Graph::Graph() : arcs_(), nodes_() {}

std::size_t Graph::getAmountOfSuccessors() const {
  std::size_t amount = 0;
  for (const auto &[_, node] : nodes_) amount += node->getAmountOfSuccessors();
  return amount;
}

unsigned int Graph::getArcId(
  const std::string &id, const int arc_index, const bool &is_successor
) const {
  if (is_successor) return nodes_.at(id)->getSuccessorArc(arc_index)->getId();
  return nodes_.at(id)->getPredecessorArc(arc_index)->getId();
}

const std::vector<std::string> Graph::getNodesId() const {
  std::vector<std::string> nodes_id;
  for (const auto &[id, _] : nodes_) nodes_id.push_back(id);
  return nodes_id;
}

void Graph::addArc(const std::string &from_id, const std::string &to_id, const int cost) {
  auto from_node = searchNode(from_id);
  auto to_node = searchNode(to_id);
  arcs_.emplace_back(from_node, to_node, cost);
}

std::shared_ptr<Node> Graph::searchNode(const std::string &id) {
  auto it = nodes_.find(id);
  if (it != nodes_.end()) return it->second;
  auto node = std::make_shared<Node>(id);
  nodes_.insert(std::make_pair(id, node));
  return node;
}

}  // namespace emir