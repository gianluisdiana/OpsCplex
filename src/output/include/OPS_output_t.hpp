#ifndef _EMIR_OPS_OUTPUT_HPP_
#define _EMIR_OPS_OUTPUT_HPP_

#include <vector>

#include <OPS_input_t.hpp>
#include <matrix.hpp>

namespace emir {

class OPS_output_t {
 public:
  const OpsInput &I_;

  GOMA::matrix<int> x_;
  std::vector<int> y_;
  std::vector<double> s_;
  std::vector<double> h_;

  GOMA::matrix<int> t_cost_;

  bool optimal_;
  bool found_;

 public:
  OPS_output_t(const OpsInput &I);

  virtual ~OPS_output_t();

  bool set(
    const std::vector<double> &x, const std::vector<double> &y,
    const std::vector<double> &s, bool optimal = true
  );

  int get_x(int k, int i, int j) const;

  virtual int &set_x(int k, int i, int j);

  int get_obj() const;
  double length() const;

  int n_customers() const;

  std::ostream &write(std::ostream &os) const;

  void write_statistics(std::ostream &os) const;

  void init_t_cost();

  bool check();

 private:
  // Maximum time margin for checking the feasibility of the solution
  static const double kMaxTimeMargin;
};

}  // namespace emir

#endif  // _EMIR_OPS_OUTPUT_HPP_