#ifndef _EMIR_OPS_SOLVER_HPP_
#define _EMIR_OPS_SOLVER_HPP_

#include <OPS_output_t.hpp>

namespace emir {

class OpsSolver {
 public:
  OpsSolver(const OpsInput &input, double eps) :
    input_(input), output_(input), tol_(eps) {}

  virtual ~OpsSolver() {}

  virtual void solve(std::ostream &r_os) = 0;

 protected:
  const OpsInput &input_;
  OpsOutput output_;
  const double tol_;
};

}  // namespace emir

#endif  // _EMIR_OPS_SOLVER_HPP_