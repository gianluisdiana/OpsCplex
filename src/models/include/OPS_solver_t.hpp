#ifndef _EMIR_OPS_SOLVER_HPP_
#define _EMIR_OPS_SOLVER_HPP_

#include <cassert>
#include <iostream>

#include <OPS_input_t.hpp>
#include <OPS_output_t.hpp>

namespace emir {

class OPS_solver_t {
 public:
  const OPS_input_t *I_;
  OPS_output_t &O_;
  const double tol_;

 public:
  OPS_solver_t(const OPS_input_t *I, OPS_output_t &O, double eps) :
    I_(I), O_(O), tol_(eps) {}

  virtual ~OPS_solver_t(void) {}

  virtual void
  solve(std::ostream &r_os, double ub = 1E10, bool root_node = false) = 0;
};

}  // namespace emir

#endif  // _EMIR_OPS_SOLVER_HPP_