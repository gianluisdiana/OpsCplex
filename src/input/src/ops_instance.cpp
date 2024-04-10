// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_instance.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the input instance for the
 * O.P.S. problem, representing the math model.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

#include <functions.hpp>
#include <ops_instance.hpp>

namespace emir {

OpsInstance::OpsInstance(double scaling_factor) :
  date_stamp_ {}, type_ {}, alpha_ {-1}, time_limit_ {0},
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
  time_to_process_ = json_instance["T"].get<std::vector<std::vector<int>>>();
  name_ = json_instance["id"][0].get<std::string>();
  date_stamp_ = stringToDateStamp(json_instance["id"][1].get<std::string>());
  type_ = json_instance["type"].get<int>();
  priorities_ = json_instance["b"].get<std::vector<int>>();
  objects_per_sliding_bar_ =
    json_instance["Jk"].get<std::vector<std::vector<int>>>();
  resetSlidingBarsPerObject();
  alpha_ = json_instance["alpha"].get<double>();
  time_limit_ = json_instance["L"].get<int>();
}

void OpsInstance::resetSlidingBarsPerObject() {
  sliding_bars_per_object_.clear();
  sliding_bars_per_object_.resize(getAmountOfObjects());
  for (auto sliding_bar_idx = 0; sliding_bar_idx < getAmountOfSlidingBars();
       ++sliding_bar_idx) {
    for (const auto node_id : getObjectsPerSlidingBar(sliding_bar_idx)) {
      sliding_bars_per_object_[node_id].push_back(sliding_bar_idx);
    }
  }
}

}  // namespace emir