#pragma once 

#include "OPS_input_t.hpp"
#include "OPS_output_t.hpp"

#include <cassert>
#include <iostream>
using namespace std;


namespace EMIR
{

class OPS_solver_t
{
public:

    const OPS_input_t*  I_;
    OPS_output_t&       O_;
    const double        tol_;

public:
    OPS_solver_t(const OPS_input_t* I, OPS_output_t& O, double eps):
    I_(I),
    O_(O),
    tol_(eps)
    {}
    
    
    virtual ~OPS_solver_t( void ) {}

    virtual void solve(ostream& r_os, double ub = 1E10, bool root_node = false) = 0;
    

};



}