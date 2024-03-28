#include <cassert>
#include <cmath>
#include <iomanip>
#include <set>

#include <ops_output.hpp>

namespace emir {

const double OpsOutput::kMaxTimeMargin = 1e-2;

OpsOutput::OpsOutput(const OpsInput &input) :
  input_(input), x_(input.getN() * input.getM(), input.getN()),
  y_(input.getN(), false), s_(input.getN(), 0),
  h_(input.getN(), double(input.getL()) / input.getScalingFactor()),
  time_elapsed_(-1), optimal_(false), found_(false) {}

// ------------------------------- Operators ------------------------------- //

std::ostream &operator<<(std::ostream &output_stream, const OpsOutput &output) {
  output_stream << nlohmann::json({{"x", output.x_.data()},
                                   {"y", output.y_},
                                   {"s", output.s_},
                                   {"profit", output.getTotalProfit()},
                                   {"time_elapsed", output.time_elapsed_}}
  ).dump(2);
  return output_stream;
}

// ------------------------------- Setters --------------------------------- //

void OpsOutput::setX(const std::vector<double> &usedArcs) {
  x_.init(false);
  for (int k = 0; k < input_.getM(); ++k) {
    const auto &graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const double value = std::round(usedArcs[arc.getId()]);
      assert(value == 1 || value == 0);
      if (value == 0.0) { continue; }
      const int origin_id = std::stoi(arc.getOriginId());
      const int destination_id = std::stoi(arc.getDestinationId());
      setXAsTrue(k, origin_id, destination_id);
    }
  }
}

void OpsOutput::setY(const std::vector<double> &visitedObjects) {
  const auto amountOfObjects = input_.getN();
  y_[0] = true;
  y_[amountOfObjects - 1] = true;
  if (visitedObjects.empty()) { return; }
  for (int idx = 1; idx < amountOfObjects - 1; ++idx) {
    const double value = visitedObjects[idx - 1];
    assert(value == 1.0 || value == 0.0);
    y_[idx] = (value == 1.0);
  }
}

void OpsOutput::setS(const std::vector<double> &timeAtObjects) {
  const auto amountOfObjects = input_.getN();
  s_[0] = 0;
  if (timeAtObjects.empty()) { return; }
  for (auto idx = 1; idx < amountOfObjects; ++idx) {
    const double value = timeAtObjects[idx - 1];
    assert(value >= 0);
    s_[idx] = value / input_.getScalingFactor();
  }
}

// ------------------------------- Getters --------------------------------- //

std::size_t OpsOutput::getObjectsVisited() const {
  std::size_t amount = 0;
  for (std::size_t idx = 1; idx < y_.size() - 1; ++idx) {
    if (y_[idx]) { ++amount; }
  }
  return amount;
}

int OpsOutput::getTotalProfit() const {
  assert(!s_.empty() && s_[0] != -1);
  int solutionValue = 0;
  for (std::size_t idx = 0; idx < y_.size(); ++idx) {
    if (y_[idx]) { solutionValue += input_.getB(idx); }
  }
  return solutionValue;
}

// ---------------------------- Utility Methods ---------------------------- //

void OpsOutput::check() const {
  checkArcs();
  checkTime();
}

void OpsOutput::checkArcs() const {
  const auto amountOfObjects = input_.getN();
  const auto amountOfSlidingBars = input_.getM();
  std::vector<int> amount_of_arrives(amountOfObjects, 0);
  std::vector<int> amount_of_departures(amountOfObjects, 0);

  for (int k = 0; k < amountOfSlidingBars; k++) {
    const auto &graph = input_.getGraph(k);
    for (const auto &arc : graph.getArcs()) {
      const auto &origin_id = std::stoi(arc.getOriginId());
      const auto &destination_id = std::stoi(arc.getDestinationId());
      if (getX(k, origin_id, destination_id)) {
        amount_of_arrives[destination_id]++;
        amount_of_departures[origin_id]++;
      }
    }
  }

  // The first node has to be used in each sliding bar, same with the last node
  assert(
    amount_of_departures[0] == amountOfSlidingBars &&
    amount_of_arrives[amountOfObjects - 1] == amountOfSlidingBars
  );

  for (std::size_t idx = 1; idx < amountOfObjects - 1; ++idx) {
    // A node must have the same number of arrival and departure arcs
    assert(amount_of_departures[idx] == amount_of_arrives[idx]);
    // The node must be visited in order to have arrival / departure arcs
    assert(y_[idx] == (amount_of_arrives[idx] > 0));
  }
}

void OpsOutput::checkTime() const {
  const double realMaximumTime =
    double(input_.getL()) / input_.getScalingFactor();
  for (const auto &timeAtObject : s_) {
    if (timeAtObject > realMaximumTime + OpsOutput::kMaxTimeMargin) {
      assert(timeAtObject <= realMaximumTime + OpsOutput::kMaxTimeMargin);
      exit(1);
    }
  }
}

}  // namespace emir