#include <arc.hpp>
#include <node.hpp>

namespace emir {

Node::Node(const std::string &id) : id_(id), successors_(), predecessors_() {}

const std::vector<unsigned int>
Node::getArcsId(const NodeArcPairs &nodesWithArcs) const {
  std::vector<unsigned int> arcs_id;
  for (const auto &[_, arc] : nodesWithArcs) {
    arcs_id.push_back(arc->getId());
  }
  return arcs_id;
}

}  // namespace emir