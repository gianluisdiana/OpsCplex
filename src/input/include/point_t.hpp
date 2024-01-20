#pragma once

#include <utility>
#include <iostream>
#include <iomanip>

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

namespace EMIR
{

class point_t: public pair<double,double>
{
public:
    point_t(void):
        pair<double,double>(0,0) {}

    point_t(double a, double b):
        pair<double,double>(a, b) {}

    virtual ~point_t(void) {}

    ostream& write(ostream& os) const;
    
    inline const double get_x(void) const {return first;}
    inline const double get_y(void) const {return second;}

    void get_json(json& point) const;
    void set_json(const json& point);
};


}


