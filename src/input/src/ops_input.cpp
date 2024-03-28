// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file OPS_input_t.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.2.0
 * @date January 21, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the input instance for the
 *  O.P.S. problem, representing the math model with a graph for each sliding bar.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <ops_input.hpp>

namespace emir {

OpsInput::OpsInput() = default;

OpsInput::~OpsInput() {
  Arc::resetIdCounter();
}

// -------------------------------- Getters -------------------------------- //

unsigned int OpsInput::getMaxArc() const {
  unsigned int max_arc = 0;
  for (const auto &graph : graphs_) {
    for (const auto &arc : graph.getArcs()) {
      max_arc = std::max<unsigned int>(max_arc, arc.getCost());
    }
  }
  return max_arc;
}

// ------------------------------ Operators ------------------------------ //

std::istream &operator>>(std::istream &input_stream, OpsInput &ops_input) {
  input_stream >> static_cast<OpsInstance &>(ops_input);
  ops_input.createGraphArcs();
  return input_stream;
}

// --------------------------- Private Methods --------------------------- //

void OpsInput::createGraphArcs() {
  const auto amountOfObjects = (int)getN();
  const auto amountOfSlidingBars = getM();
  graphs_.resize(amountOfSlidingBars);
  for (auto k = 0; k < amountOfSlidingBars; ++k) {
    auto graph = Graph();
    graph.addArc(0, amountOfObjects - 1, 0);
    const auto &objectsObservedByK = getJk(k);
    for (const auto &objectI : objectsObservedByK) {
      graph.addArc(0, objectI, getT(0, objectI));
      graph.addArc(
        objectI, amountOfObjects - 1, getT(objectI, amountOfObjects - 1)
      );
      for (const auto &objectJ : objectsObservedByK) {
        if (objectI != objectJ) {
          graph.addArc(objectI, objectJ, getT(objectJ, objectI));
        }
      }
    }
    graphs_[k] = graph;
  }
}

}  // namespace emir