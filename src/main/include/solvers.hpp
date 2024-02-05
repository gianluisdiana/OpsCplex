#ifndef _SOLVERS_HPP_
#define _SOLVERS_HPP_

#include <cstdio>
#include <fstream>
#include <iostream>

#include <OPS_bc1.hpp>

using solver_ptr = void (*)(
  const emir::OpsInput *I, double tol, emir::OPS_output_t &O, std::ostream &os1,
  std::ostream &os2
);

void solver1(
  const emir::OpsInput *I, double tol, emir::OPS_output_t &O, std::ostream &os1,
  std::ostream &os2
) {
  emir::OPS_cplex_solver1 solver_bc(I, O, tol);

  solver_bc.solve(os1);
}

solver_ptr solver_array[] = {solver1};

#endif  // _SOLVERS_HPP_