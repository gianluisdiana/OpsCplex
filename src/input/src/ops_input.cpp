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
  const auto amount_of_objects = getN();
  const auto amount_of_sliding_bars = getM();
  graphs_.resize(amount_of_sliding_bars);
  for (auto sliding_bar_idx = 0; sliding_bar_idx < amount_of_sliding_bars;
       ++sliding_bar_idx) {
    auto &graph = graphs_[sliding_bar_idx];
    graph.addArc(0, amount_of_objects - 1, 0);
    const auto &objects_in_sliding_bar = getJk(sliding_bar_idx);
    for (const auto &origin_id : objects_in_sliding_bar) {
      graph.addArc(0, origin_id, getT(0, origin_id));
      graph.addArc(
        origin_id, amount_of_objects - 1, getT(origin_id, amount_of_objects - 1)
      );
      for (const auto &destination_id : objects_in_sliding_bar) {
        if (origin_id != destination_id) {
          graph.addArc(
            origin_id, destination_id, getT(origin_id, destination_id)
          );
        }
      }
    }
  }
}

}  // namespace emir