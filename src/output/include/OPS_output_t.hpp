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

  OPS_output_t(const OPS_output_t &O);

  virtual ~OPS_output_t(void);

  virtual int get_n() const {
    return I_.getN();
  }

  virtual int get_m() const {
    return I_.getM();
  }

  bool set(
    const std::vector<double> &x, const std::vector<double> &y,
    const std::vector<double> &s, bool optimal = true
  );
  bool set(
    const std::vector<double> &x, const std::vector<double> &y,
    bool optimal = true
  );
  void set(std::vector<double> &x, std::vector<int> &y);

  int get_x(int k, int i, int j) const;
  virtual int &set_x(int k, int i, int j);
  virtual int &set_x(GOMA::matrix<int> &M, int k, int i, int j) const;
  int get_y(int j) const;
  virtual int &set_y(int i);
  double get_s(int j) const;

  void get_Jk(std::vector<int> &Jk, int k) const;

  int get_obj(void) const;
  double length(int k) const;
  double length(void) const;

  double slack(int i) const;
  double slack(void) const;

  bool found(void) const {
    return found_;
  }

  bool optimal(void) const {
    return optimal_;
  }

  int n_customers(void) const;

  const OpsInput &get_input(void) const {
    return I_;
  }

  std::ostream &write(std::ostream &os) const;

  void write_statistics(std::ostream &os) const;

  int get_next(int i, int k) const;
  int get_prev(int i, int k) const;

  void init_t_cost(void);

  void set(const OPS_output_t &O, int k);
  void set(const OPS_output_t &O);
  void setADD(const OPS_output_t &O);

  bool idle(int k) const {
    const int n = get_n();
    return (x_(1 + k * n, n) == 1);
  }

  bool check(void);

  // protected:

 private:
  // Maximum time margin for checking the feasibility of the solution
  static const double kMaxTimeMargin;
};

}  // namespace emir

#endif  // _EMIR_OPS_OUTPUT_HPP_