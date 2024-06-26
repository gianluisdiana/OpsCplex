// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_input.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the input instance for the
 *  O.P.S. problem, with a graph for each sliding bar.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <algorithm>
#include <iostream>

#include <arc.hpp>
#include <graph.hpp>
#include <ops_input.hpp>
#include <ops_instance.hpp>

namespace emir {

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
  ops_input.createGraphs();
  return input_stream;
}

// --------------------------- Private Methods --------------------------- //

void OpsInput::createGraphs() {
  const auto amount_of_sliding_bars = getAmountOfSlidingBars();
  graphs_.resize(amount_of_sliding_bars);
  for (auto graph_idx = 0; graph_idx < amount_of_sliding_bars; ++graph_idx) {
    addGraphArcs(graph_idx);
  }
}

void OpsInput::addGraphArcs(const int graph_idx) {
  auto &graph = graphs_[graph_idx];
  const auto amount_of_objects = (unsigned int)getAmountOfObjects();
  const auto &objects_in_sliding_bar = getObjectsPerSlidingBar(graph_idx);
  graph.addArc({.origin_id = 0, .destination_id = amount_of_objects - 1}, 0);
  for (const auto &origin_id : objects_in_sliding_bar) {
    graph.addArc(
      {.origin_id = 0, .destination_id = origin_id},
      getTimeToProcess({0, origin_id})
    );
    graph.addArc(
      {.origin_id = origin_id, .destination_id = amount_of_objects - 1},
      getTimeToProcess({origin_id, amount_of_objects - 1})
    );
    for (const auto &destination_id : objects_in_sliding_bar) {
      if (origin_id == destination_id) { continue; }
      graph.addArc(
        {.origin_id = origin_id, .destination_id = destination_id},
        getTimeToProcess({origin_id, destination_id})
      );
    }
  }
}

}  // namespace emir