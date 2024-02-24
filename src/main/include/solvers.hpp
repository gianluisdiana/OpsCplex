#ifndef _SOLVERS_HPP_
#define _SOLVERS_HPP_

#include <OPS_solver_t.hpp>

template<
  typename T,
  typename std::enable_if<std::is_base_of<emir::OPS_solver_t, T>::value>::type
    * = nullptr>
void solve(
  const emir::OpsInput *I, double tol, emir::OPS_output_t &O, std::ostream &os1,
  std::ostream &os2
) {
  T solver_bc(I, O, tol);
  solver_bc.solve(os1);
}

#endif  // _SOLVERS_HPP_