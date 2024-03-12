/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file OPS_instance_t.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.1.1
 * @date January 21, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the basic description of an instance for the O.P.S.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex
 * @endlink
 * @see Selective routing problem with synchronization @link
 * https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1
 * @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link
 * https://google.github.io/styleguide/cppguide.html @endlink
 */

#ifndef _EMIR_OPS_INSTANCE_HPP_
#define _EMIR_OPS_INSTANCE_HPP_

#include <string>

#include <matrix.hpp>

namespace emir {

/** @brief Represents a basic instance for the O.P.S. */
class OpsInstance : JsonInterface {
 public:
  // Constant to represent an infinite time to process an object and go to
  // another
  static const unsigned int kInfiniteTime;

  OpsInstance();
  virtual ~OpsInstance();

  // ------------------------------ Getters -------------------------------- //

  /** @brief Get the amount of objects to visualize */
  inline int getN() const {
    return b_.size();
  }

  /** @brief Get the amount of sliding bars */
  inline int getM() const {
    return Jk_.size();
  }

  /**
   * @brief Get the objects that can be observed by the 'k' sliding bar
   *
   * @param k The index of the sliding bar
   * @return The objects that can be observed by the sliding bar selected
   */
  inline const std::vector<int> &getJk(int k) const {
    return Jk_[k];
  }

  /**
   * @brief Get the profit (or priority) for the object 'j'
   *
   * @param j The index of the object
   * @return The profit (or priority) for the object selected
   */
  inline int getB(int j) const {
    return b_[j];
  }

  /** @brief Returns the time limit to use the telescope */
  inline int getL() const {
    return L_;
  }

  /**
   * @brief Get the time spent to process the object 'origin' and go to the
   * object 'destiny'
   *
   * @param origin_index The index of the origin node
   * @param destiny_index The index of the destiny node
   */
  inline int getT(const int origin_index, const int destiny_index) const {
    return T_(origin_index, destiny_index);
  }

  /** @brief Gives read-only access to the scaling factor */
  inline double getScalingFactor() const {
    return scal_factor_;
  }

  // ------------------------------ Setters -------------------------------- //

  /**
   * @brief Sets the element in the i-th row and j-th column of the time matrix
   * to an infinite value (represents a transition that cannot be done).
   *
   * @param i The index of the row
   * @param j The index of the column
   */
  inline void setTOutOfRange(const int i, const int j) {
    T_(i, j) = OpsInstance::kInfiniteTime;
  }

  /**
   * @brief Sets the element in the i-th row and j-th column of the time matrix
   * to 0 (represents a transition with no time spent).
   *
   * @param i The index of the row
   * @param j The index of the column
   */
  inline void setTZero(const int i, const int j) {
    T_(i, j) = 0;
  }

  // ---------------------------- Statistcs Data --------------------------- //
  // TODO: ERASE

  void writeStatistics(std::ostream &os) const;
  void writeStatisticsHdr(std::ostream &os) const;

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the >> operator to read an instance from a json file.
   *
   * @param is Represents the inflow
   * @param ops_instance The OPS instance to read from the inflow
   * @return The inflow with the instance read
   */
  friend std::istream &operator>>(std::istream &is, OpsInstance &ops_instance);

 private:
  // ----------------------------------------------------------------------- //
  // ----------------------------- Attributes ------------------------------ //
  // ----------------------------------------------------------------------- //

  // ------------------------- Instance Attributes ------------------------- //

  // Name of the instance, typically the name of the file
  std::string name_;
  // Date stamp of the instance creation
  std::time_t date_stamp_;
  // Type of instance generation @see instance_code_type.hpp
  int type_;

  // -------------------- Mathematical model attributes -------------------- //

  // Objects that can be observed by the 'k' sliding bar
  std::vector<std::vector<int>> Jk_;
  // Necessary sliding bars to observe the object 'j' (inverse of Jk_)
  std::vector<std::vector<int>> Kj_;
  // Time matrix to represent the time spent to process the object 'i'
  // and go to the object 'j'
  Matrix<int> T_;
  // Profit (or priority) for each job
  std::vector<int> b_;
  // Time limit to use the telescope
  int L_;

  // --------------------------- Extra attributes -------------------------- //

  double alpha_; /**< Percentage of the total tour */
  double scal_factor_;

  // ----------------------------------------------------------------------- //
  // ------------------------------- Methods ------------------------------- //
  // ----------------------------------------------------------------------- //

  // TODO: ERASE
  /**
   * @brief Formats the instance to a json file
   *
   * @return The json file with the ops information
   */
  const nlohmann::json toJson() const override;

  /**
   * @brief Set the OPS instance from a json file
   *
   * @param json_instance The json file with the ops information
   */
  void setFromJson(const nlohmann::json &json_instance) override;

  /**
   * @brief Resets the Kj matrix and fills it with the correct values
   */
  void resetKjMatrix();
};

}  // namespace emir

#endif  // _EMIR_OPS_INSTANCE_HPP_