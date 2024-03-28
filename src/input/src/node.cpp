#include <arc.hpp>
#include <node.hpp>

namespace emir {

Node::Node(std::string node_id) :
  id_ {std::move(node_id)} {}

std::vector<unsigned int>
Node::getArcsId(const NodeArcPairs &nodesWithArcs) {
  std::vector<unsigned int> arcs_id;
  for (const auto &[_, arc] : nodesWithArcs) {
    arcs_id.push_back(arc->getId());
  }
  return arcs_id;
}

}  // namespace emir