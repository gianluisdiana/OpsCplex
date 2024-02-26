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

}  // namespace emir