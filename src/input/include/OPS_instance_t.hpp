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

#define N_ITEM 2

namespace emir {

enum { NAME, STAMP };

/** @brief Represents a basic instance for the O.P.S. */
class OPS_instance_t {
 public:
  // Constant to represent an infinite time to process an object and go to
  // another
  static const unsigned int kInfiniteTime;

  OPS_instance_t(void);
  virtual ~OPS_instance_t(void);

  std::istream &read(std::istream &is);
  std::ostream &write(std::ostream &os) const;

  inline const std::string &get_instance_name(void) const {
    return id_[NAME];
  }

  inline const std::string &get_instance_stamp(void) const {
    return id_[STAMP];
  }

  inline int get_n(void) const {
    return b_.size();
  }

  inline int get_m(void) const {
    return Jk_.size();
  }

  inline const std::vector<int> &get_Jk(int k) const {
    return Jk_[k];
  }

  inline int get_b(int j) const {
    return b_[j];
  }

  inline int get_L(void) const {
    return L_;
  }

  inline const GOMA::matrix<int> &get_T(void) const {
    return T_;
  }

  inline double get_scal_factor(void) const {
    return scal_factor_;
  }

  int get_max_Jk(void) const;

  const std::vector<std::vector<int>> &get_Kj(void) const {
    return Kj_;
  }

  void set(
    const std::string &source_name, const std::string &source_stamp,
    const std::string &desc, int type, const std::vector<std::vector<int>> &Jk,
    const GOMA::matrix<int> &T, const std::vector<int> &b
  );
  void set(const OPS_instance_t &O);
  void set(
    const std::string &name, const std::string &stamp, const std::string &desc
  );
  void set_L(double alpha, int L);

  void write_statistics(std::ostream &os) const;
  void write_statistics_hdr(std::ostream &os) const;

 private:
  // ----------------------------------------------------------------------- //
  // ----------------------------- Attributes ------------------------------ //
  // ----------------------------------------------------------------------- //

  // ------------------------- Instance Attributes ------------------------- //

  std::vector<std::string> id_; /**< Id of the instance Id of the source target
                                   points @see target_set_t.hpp */
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

  void get_json(json &instance) const;
  void set_json(const json &instance);

  void make_Kj(void);
};

}  // namespace emir

std::istream &operator>>(std::istream &is, emir::OPS_instance_t &input);
std::ostream &operator<<(std::ostream &os, const emir::OPS_instance_t &input);

#endif  // _EMIR_OPS_INSTANCE_HPP_