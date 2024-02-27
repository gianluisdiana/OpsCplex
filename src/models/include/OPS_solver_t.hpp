#ifndef _EMIR_OPS_SOLVER_HPP_
#define _EMIR_OPS_SOLVER_HPP_

#include <OPS_output_t.hpp>

namespace emir {

class OPS_solver_t {
 public:
  OPS_solver_t(const OpsInput &I, double eps) : I_(I), O_(I), tol_(eps) {}

  virtual ~OPS_solver_t() {}

  virtual void solve(std::ostream &r_os) = 0;

 protected:
  const OpsInput &I_;
  OPS_output_t O_;
  const double tol_;
};

}  // namespace emir

#endif  // _EMIR_OPS_SOLVER_HPP_