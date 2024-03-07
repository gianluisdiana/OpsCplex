#include <arc.hpp>
#include <node.hpp>

namespace emir {

std::shared_ptr<Arc> Node::getSuccessorArc(const int &i) const {
  auto it = successors_.begin();
  std::advance(it, i);
  return it->second;
}

std::shared_ptr<Arc> Node::getPredecessorArc(const int &i) const {
  auto it = predecessors_.begin();
  std::advance(it, i);
  return it->second;
}

const std::vector<unsigned int> Node::getArcsId(
  const std::map<std::shared_ptr<Node>, std::shared_ptr<Arc>> &nodesWithArcs
) const {
  std::vector<unsigned int> arcs_id;
  for (const auto &[_, arc] : nodesWithArcs) {
    arcs_id.push_back(arc->getId());
  }
  return arcs_id;
}

}  // namespace emir