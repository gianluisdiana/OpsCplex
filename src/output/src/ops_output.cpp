// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_output.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the output generated after solving
 * the O.P.S. problem.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <functions.hpp>
#include <graph.hpp>
#include <ops_error.hpp>
#include <ops_input.hpp>
#include <ops_output.hpp>

namespace emir {

const double OpsOutput::kMaxTimeMargin = 1e-2;

OpsOutput::OpsOutput(const OpsInput &input) :
  input_(std::make_unique<OpsInput>(input)),
  used_arcs_(input.getAmountOfSlidingBars()),
  observed_objects_(input.getAmountOfObjects(), false),
  time_at_objects_(input.getAmountOfObjects(), 0) {
  for (auto &used_arc : used_arcs_) {
    used_arc.resize(input.getAmountOfObjects());
  }
}

OpsOutput::OpsOutput(const OpsOutput &output) {
  this->operator=(output);
}

// ------------------------------- Setters --------------------------------- //

void OpsOutput::setUsedArcs(const std::vector<double> &used_arcs) {
  for (int graph_idx = 0; graph_idx < input_->getAmountOfSlidingBars();
       ++graph_idx) {
    const auto &graph = input_->getGraph(graph_idx);
    for (const auto &arc : graph.getArcs()) {
      const double value = std::round(used_arcs[arc.getId()]);
      if (isEqual(value, 0.0)) { continue; }
      if (!isEqual(value, 1.0)) {
        throw OpsError(
          "Invalid value for used arc: {}. It must be 1 or 0.", value
        );
      }
      used_arcs_[graph_idx][arc.getOriginId()] = arc.getDestinationId();
    }
  }
}

void OpsOutput::setObservedObjects(const std::vector<double> &observed_objects
) {
  const auto amount_of_objects = input_->getAmountOfObjects();
  observed_objects_[0] = true;
  observed_objects_[amount_of_objects - 1] = true;
  if (observed_objects.empty()) { return; }
  for (int idx = 1; idx < amount_of_objects - 1; ++idx) {
    const double value = observed_objects[idx - 1];
    if (!isEqual(value, 0.0) && !isEqual(value, 1.0)) {
      throw OpsError(
        "Invalid value for visited object: {}. It must be 1 or 0.", value
      );
    }
    observed_objects_[idx] = isEqual(value, 1.0);
  }
}

void OpsOutput::setTimeAtObjects(const std::vector<double> &time_at_objects) {
  const auto amount_of_objects = input_->getAmountOfObjects();
  time_at_objects_[0] = 0;
  if (time_at_objects.empty()) { return; }
  for (auto idx = 1; idx < amount_of_objects; ++idx) {
    const double value = time_at_objects[idx - 1];
    if (value < 0) {
      throw OpsError(
        "Invalid value for object {} with time {}. It must be non-negative.",
        value, idx
      );
    }
    time_at_objects_[idx] = value / input_->getScalingFactor();
  }
}

// ------------------------------- Operators ------------------------------- //

OpsOutput &OpsOutput::operator=(const OpsOutput &output) {
  if (this == &output) { return *this; }
  input_ = std::make_unique<OpsInput>(*output.input_);
  used_arcs_ = output.used_arcs_;
  observed_objects_ = output.observed_objects_;
  time_at_objects_ = output.time_at_objects_;
  time_elapsed_ = output.time_elapsed_;
  return *this;
}

std::ostream &operator<<(std::ostream &output_stream, const OpsOutput &output) {
  output_stream << nlohmann::json({{"x", output.used_arcs_},
                                   {"y", output.observed_objects_},
                                   {"s", output.time_at_objects_},
                                   {"profit", output.getTotalProfit()},
                                   {"time_elapsed", output.time_elapsed_}}
  ).dump(2);
  return output_stream;
}

// ------------------------------- Getters --------------------------------- //

long OpsOutput::getTotalProfit() const {
  long total_profit = 0;
  for (auto idx = 0; idx < observed_objects_.size(); ++idx) {
    if (observed_objects_[idx]) { total_profit += input_->getPriority(idx); }
  }
  return total_profit;
}

// ---------------------------- Utility Methods ---------------------------- //

std::pair<std::vector<int>, std::vector<int>>
OpsOutput::countArrivesAndDepartures() const {
  const auto amount_of_objects = input_->getAmountOfObjects();
  const auto amount_of_sliding_bars = input_->getAmountOfSlidingBars();
  std::vector<int> amount_of_arrives(amount_of_objects, 0);
  std::vector<int> amount_of_departures(amount_of_objects, 0);

  for (int graph_idx = 0; graph_idx < amount_of_sliding_bars; ++graph_idx) {
    const auto &graph = input_->getGraph(graph_idx);
    for (const auto &arc : graph.getArcs()) {
      const ArcEndpoints arc_endpoints {
        .origin_id = arc.getOriginId(), .destination_id = arc.getDestinationId()
      };
      if (arcWasUsed(graph_idx, arc_endpoints)) {
        amount_of_arrives[arc_endpoints.destination_id]++;
        amount_of_departures[arc_endpoints.origin_id]++;
      }
    }
  }
  return {amount_of_arrives, amount_of_departures};
}

// -------------------------------- Checks -------------------------------- //

void OpsOutput::check() const {
  checkArcs();
  checkTime();
}

void OpsOutput::checkArcs() const {
  const auto amount_of_objects = input_->getAmountOfObjects();
  const auto amount_of_sliding_bars = input_->getAmountOfSlidingBars();
  const auto [amount_of_arrives, amount_of_departures] =
    countArrivesAndDepartures();

  if (amount_of_departures[0] != amount_of_sliding_bars) {
    throw OpsError(
      "The first node must be visited in each sliding bar. It has {} visits.",
      amount_of_departures[0]
    );
  }
  if (amount_of_arrives[amount_of_objects - 1] != amount_of_sliding_bars) {
    throw OpsError(
      "The last node must be visited in each sliding bar. It has {} visits.",
      amount_of_arrives[amount_of_objects - 1]
    );
  }

  for (std::size_t idx = 1; idx < amount_of_objects - 1; ++idx) {
    if (amount_of_departures[idx] != amount_of_arrives[idx]) {
      throw OpsError(
        "Node {} must have the same number of arrival and departure arcs. "
        "It has {} arrivals and {} departures.",
        idx, amount_of_arrives[idx], amount_of_departures[idx]
      );
    }
    if (observed_objects_[idx] != (amount_of_arrives[idx] > 0)) {
      throw OpsError(
        "Node {} must be visited in order to have arrival / departure arcs.",
        idx
      );
    }
  }
}

void OpsOutput::checkTime() const {
  if (time_at_objects_.empty() || time_at_objects_[0] == -1) {
    throw OpsError("The time spent at each node must be set.");
  }
  const double real_maximum_time =
    double(input_->getTimeLimit()) / input_->getScalingFactor();
  for (const auto &time_at_object : time_at_objects_) {
    if (time_at_object > real_maximum_time + OpsOutput::kMaxTimeMargin) {
      throw OpsError(
        "The time spent at moment of visiting each node must be less than the "
        "maximum time. The maximum time is {} and the time spent is {}.",
        real_maximum_time, time_at_object
      );
    }
  }
}

}  // namespace emir