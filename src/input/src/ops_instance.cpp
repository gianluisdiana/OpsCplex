#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

#include <functions.hpp>
#include <ops_instance.hpp>

namespace emir {

OpsInstance::OpsInstance(double scaling_factor) :
  date_stamp_ {}, type_ {}, alpha_ {-1}, L_ {0},
  scaling_factor_ {scaling_factor} {}

// ------------------------------- Operators ------------------------------- //

std::istream &
operator>>(std::istream &input_stream, OpsInstance &ops_instance) {
  nlohmann::json json_instance;
  input_stream >> json_instance;
  ops_instance.setFromJson(json_instance);
  return input_stream;
}

// ---------------------------- Private Methods ----------------------------- //

void OpsInstance::setFromJson(const nlohmann::json &json_instance) {
  T_ = json_instance["T"].get<std::vector<std::vector<int>>>();
  name_ = json_instance["id"][0].get<std::string>();
  date_stamp_ = stringToDateStamp(json_instance["id"][1].get<std::string>());
  type_ = json_instance["type"].get<int>();
  b_ = json_instance["b"].get<std::vector<int>>();
  Jk_ = json_instance["Jk"].get<std::vector<std::vector<int>>>();
  resetKjMatrix();
  alpha_ = json_instance["alpha"].get<double>();
  L_ = json_instance["L"].get<int>();
}

void OpsInstance::resetKjMatrix() {
  Kj_.clear();
  Kj_.resize(getN());
  for (auto sliding_bar_idx = 0; sliding_bar_idx < getM(); ++sliding_bar_idx) {
    for (const auto node_id : getJk(sliding_bar_idx)) {
      Kj_[node_id].push_back(sliding_bar_idx);
    }
  }
}

}  // namespace emir
