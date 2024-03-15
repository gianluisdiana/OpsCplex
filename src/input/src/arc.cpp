#include <arc.hpp>
#include <node.hpp>

namespace emir {

unsigned int Arc::id_counter_ = 0;

Arc::Arc(
  const std::shared_ptr<Node> &from, const std::shared_ptr<Node> &to,
  const int cost
) :
  id_(Arc::id_counter_++),
  from_(from), to_(to), cost_(cost) {
  const auto &arc = std::make_shared<Arc>(*this);
  from_->addSuccessor(to_, arc);
  to_->addPredecessor(from_, arc);
}

const std::string &Arc::getOriginId() const {
  return from_->getId();
}

const std::string &Arc::getDestinationId() const {
  return to_->getId();
}

}  // namespace emir