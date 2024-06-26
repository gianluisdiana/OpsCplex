// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_instance.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the basic description of an instance for the O.P.S.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef EMIR_OPS_INSTANCE_HPP_
#define EMIR_OPS_INSTANCE_HPP_

#include <string>

#include <nlohmann/json.hpp>

#include <matrix.hpp>

namespace emir {

/** @brief Represents a basic instance for the O.P.S. */
class OpsInstance {
 public:
  /**
   * @brief Construct a new Ops Instance object with a scaling factor
   * of 10.
   *
   * @param scaling_factor The scaling factor to use
   */
  explicit OpsInstance(double scaling_factor = 10);

  // ------------------------------ Getters -------------------------------- //

  /**
   * @brief Get the amount of objects to visualize plus the origin and ending
   * objects (used as starting and ending points)
   */
  [[nodiscard]] std::size_t getAmountOfObjects() const {
    return priorities_.size();
  }

  /** @brief Get the amount of sliding bars, normally 55 */
  [[nodiscard]] std::size_t getAmountOfSlidingBars() const {
    return objects_per_sliding_bar_.size();
  }

  /**
   * @brief Get the objects that can be observed by the 'k' sliding bar
   *
   * @param sliding_bar_idx The index of the sliding bar
   * @return The objects that can be observed by the sliding bar selected
   */
  [[nodiscard]] const std::vector<unsigned int> &
  getObjectsPerSlidingBar(const std::size_t sliding_bar_idx) const {
    return objects_per_sliding_bar_[sliding_bar_idx];
  }

  /**
   * @brief Get the profit (or priority) for the object 'j'
   *
   * @param priority_idx The index of the object
   * @return The profit (or priority) for the object selected
   */
  [[nodiscard]] int getPriority(const std::size_t priority_idx) const {
    return priorities_[priority_idx];
  }

  /** @brief Returns the time limit to use the telescope */
  [[nodiscard]] int getTimeLimit() const {
    return time_limit_;
  }

  /**
   * @brief Get the time spent to process the object 'origin' and go to the
   * object 'destiny'
   *
   * @param indexes The indexes of the origin and destiny objects
   */
  [[nodiscard]] int
  getTimeToProcess(std::pair<std::size_t, std::size_t> indexes) const {
    return time_to_process_(indexes);
  }

  /** @brief Gives read-only access to the scaling factor */
  [[nodiscard]] double getScalingFactor() const {
    return scaling_factor_;
  }

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the >> operator to read an instance from a json file.
   *
   * @param input_stream Represents the inflow
   * @param ops_instance The OPS instance to read from the inflow
   * @return The inflow with the instance read
   */
  friend std::istream &
  operator>>(std::istream &input_stream, OpsInstance &ops_instance);

 private:
  // ------------------------- Instance Attributes ------------------------- //

  // Name of the instance, typically the name of the file
  std::string name_;
  // Date stamp of the instance creation
  std::time_t date_stamp_ {};
  // Type of instance generation @see instance_code_type.hpp
  int type_ {};

  // -------------------- Mathematical model attributes -------------------- //

  // Objects that can be observed by the 'k' sliding bar
  std::vector<std::vector<unsigned int>> objects_per_sliding_bar_;
  // Time matrix to represent the time spent to process the object 'i'
  // and go to the object 'j'
  Matrix<int> time_to_process_;
  // Profit (or priority) for each job
  std::vector<int> priorities_;
  // Time limit to use the telescope
  int time_limit_ {0};

  // --------------------------- Extra attributes -------------------------- //

  // Percentage of the total time
  double alpha_ {-1};
  double scaling_factor_;

  // ------------------------------- Methods ------------------------------- //

  /**
   * @brief Set the OPS instance from a json file
   *
   * @param json_instance The json file with the ops information
   */
  void setFromJson(const nlohmann::json &json_instance);
};

}  // namespace emir

#endif  // EMIR_OPS_INSTANCE_HPP_