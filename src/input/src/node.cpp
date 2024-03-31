#include <arc.hpp>
#include <node.hpp>

namespace emir {

Node::Node(unsigned int node_id) : id_ {std::move(node_id)} {}

std::vector<unsigned int> Node::getArcsId(const NodeArcPairs &nodes_with_arcs) {
  std::vector<unsigned int> arcs_id;
  for (const auto &[_, arc] : nodes_with_arcs) {
    arcs_id.push_back(arc->getId());
  }
  return arcs_id;
}

}  // namespace emir