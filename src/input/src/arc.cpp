#include <arc.hpp>
#include <node.hpp>

namespace emir {

unsigned int Arc::id_counter_ = 0;

void Arc::resetIdCounter() {
  Arc::id_counter_ = 0;
}

Arc::Arc(
  std::shared_ptr<Node> origin, const int cost,
  std::shared_ptr<Node> destination
) :
  id_ {Arc::id_counter_++},
  origin_ {std::move(origin)}, cost_ {cost}, destination_ {
                                               std::move(destination)} {
  const auto &arc = std::make_shared<Arc>(*this);
  origin_->addSuccessor(destination_, arc);
  destination_->addPredecessor(origin_, arc);
}

const std::string &Arc::getOriginId() const {
  return origin_->getId();
}

const std::string &Arc::getDestinationId() const {
  return destination_->getId();
}

}  // namespace emir