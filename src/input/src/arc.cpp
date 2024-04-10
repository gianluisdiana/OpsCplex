// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file arc.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the arc inside a graph.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <memory>
#include <utility>

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
  id_ {Arc::id_counter_++}, origin_ {std::move(origin)}, cost_ {cost},
  destination_ {std::move(destination)} {
  const auto &arc = std::make_shared<Arc>(*this);
  origin_->addSuccessor(destination_, arc);
  destination_->addPredecessor(origin_, arc);
}

unsigned int Arc::getOriginId() const {
  return origin_->getId();
}

unsigned int Arc::getDestinationId() const {
  return destination_->getId();
}

}  // namespace emir