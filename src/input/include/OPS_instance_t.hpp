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

#include <iostream>
#include <string>
#include <vector>

#include <json.hpp>
#include <matrix.hpp>

using json = nlohmann::json;

namespace emir {

/** @brief Represents a basic instance for the O.P.S. */
class OpsInstance {
 public:
  // Constant to represent an infinite time to process an object and go to
  // another
  static const unsigned int kInfiniteTime;

  OpsInstance(void);
  virtual ~OpsInstance(void);

  // ------------------------------ Getters -------------------------------- //

  /** @brief Gets the name of the instance. */
  inline const std::string &get_instance_name(void) const {
    return name_;
  }

  /**
   * @brief Returns the date stamp of the current instance.
   *   The returned string has the following format:
   *   Www Mmm dd hh:mm:ss yyyy
   */
  inline const std::string get_instance_stamp(void) const {
    return std::ctime(&date_stamp_);
  }

  /** @brief Get the amount of objects to visualize */
  inline int get_n(void) const {
    return b_.size();
  }

  /** @brief Get the amount of sliding bars */
  inline int get_m(void) const {
    return Jk_.size();
  }

  /**
   * @brief Get the objects that can be observed by the 'k' sliding bar
   *
   * @param k The index of the sliding bar
   * @return The objects that can be observed by the sliding bar selected
   */
  inline const std::vector<int> &get_Jk(int k) const {
    return Jk_[k];
  }

  /**
   * @brief Get the profit (or priority) for the object 'j'
   *
   * @param j The index of the object
   * @return The profit (or priority) for the object selected
   */
  inline int get_b(int j) const {
    return b_[j];
  }

  /** @brief Returns the time limit to use the telescope */
  inline int get_L(void) const {
    return L_;
  }

  /** @brief Returns the full time matrix */
  inline const GOMA::matrix<int> &get_T(void) const {
    return T_;
  }

  /** @brief Gives read-only access to the scaling factor */
  inline double get_scal_factor(void) const {
    return scal_factor_;
  }

  /** @brief Returns the Kj matrix */
  inline const std::vector<std::vector<int>> &get_Kj(void) const {
    return Kj_;
  }

  int get_max_Jk(void) const;

  // ------------------------------- Setters ------------------------------- //

  void set(
    const std::string &source_name, const std::string &source_stamp,
    const std::string &desc, int type, const std::vector<std::vector<int>> &Jk,
    const GOMA::matrix<int> &T, const std::vector<int> &b
  );
  void set(const OpsInstance &O);
  void set(
    const std::string &name, const std::string &stamp, const std::string &desc
  );
  void set_L(double alpha, int L);

  // ---------------------------- Statistcs Data --------------------------- //

  void write_statistics(std::ostream &os) const;
  void write_statistics_hdr(std::ostream &os) const;

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the >> operator to read an instance from a json file.
   *
   * @param is Represents the inflow
   * @param ops_instance The OPS instance to read from the inflow
   * @return The inflow with the instance read
   */
  friend std::istream &operator>>(std::istream &is, OpsInstance &ops_instance);

  /**
   * @brief Overload of the << operator to display a json format of the
   * instance.
   *
   * @param os Represents the outflow
   * @param ops_instance The OPS instance to display
   * @return The outflow with the string that represents the state.
   */
  friend std::ostream &
  operator<<(std::ostream &os, const OpsInstance &ops_instance);

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
  GOMA::matrix<int> T_;
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

  /**
   * @brief Formats the instance to a json file
   *
   * @return The json file with the ops information
   */
  json toJson() const;

  /**
   * @brief Set the OPS instance from a json file
   *
   * @param json_instance The json file with the ops information
   */
  void setFromJson(const json &json_instance);

  /**
   * @brief Truncate the T matrix, checking if the current value of a cell
   * exceeds the maximum value allowed (the time limit to use the
   * telescope)
   */
  void truncateTMatrix();

  /**
   * @brief Resets the Kj matrix and fills it with the correct values
   */
  void make_Kj();
};

}  // namespace emir

#endif  // _EMIR_OPS_INSTANCE_HPP_