#ifndef _SOLVERS_HPP_
#define _SOLVERS_HPP_

#include <cstdio>
#include <iostream>
#include <fstream>

#include "OPS_instance_t.hpp"
#include "OPS_input_t.hpp"
#include "OPS_output_t.hpp"

#include "OPS_bc1.hpp"

using namespace emir;

typedef void (*solver_ptr)(const OPS_input_t* I, double tol, OPS_output_t &O, std::ostream &os1, std::ostream &os2);

void solver1(const OPS_input_t* I, double tol, OPS_output_t &O, std::ostream &os1, std::ostream &os2)
{
    OPS_cplex_solver1 solver_bc(I, O, tol);

    solver_bc.solve(os1);
}

solver_ptr solver_array[] = {solver1};

#endif // _SOLVERS_HPP_