#pragma once

#include "OPS_input_t.hpp"
#include "OPS_output_t.hpp"

#include "OPS_solver_t.hpp"


#include <iostream>

#define IL_STD
#include <ilcplex/ilocplex.h>

using namespace std;

namespace EMIR
{

    class OPS_cplex_solver1 : public OPS_solver_t
    {
    public:
        IloEnv env_;
        IloNumVarArray x_;
        IloNumVarArray y_;
        IloNumVarArray s_;
        IloCplex cplex_;

        double tol_;

    public:
        OPS_cplex_solver1(const OPS_input_t *I, OPS_output_t &O, double eps);
        virtual ~OPS_cplex_solver1(void);

        virtual void set_param(ostream &r_os);

        virtual void solve(ostream &r_os, double ub = 1E10, bool root_node = false);

        void set_output(OPS_output_t &output);

        void makeModel(IloModel &model);
    };

}