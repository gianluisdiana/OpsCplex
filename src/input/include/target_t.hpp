#ifndef _EMIR_TARGET_HPP_
#define _EMIR_TARGET_HPP_

#include <iostream>
#include "point_t.hpp"

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

namespace emir {

class target_t: public point_t
{
private:
    double	p_;         /**< Exposure time */
    double	b_;         /**< Priority value */

public:

    target_t(void):
        point_t(),
        p_(0),
        b_(-1) {}

    target_t(const point_t& coord, double p, double b):
        point_t(coord),
        p_(p),
        b_(b) {}
        
    target_t(const point_t& coord):
        point_t(coord),
        p_(0),
        b_(-1) {} 


    inline double  get_p(void) const
    {
        return p_;
    }
    inline double  get_b(void) const
    {
        return b_;
    }
    inline const point_t& get_coord(void) const
    {
        return *this;
    }

    virtual ~target_t(void) {}

    ostream& write(ostream& os) const;
    istream& read(istream& is);

    void get_json(json& target) const;
    void set_json(const json& target);
};

} // namespace emir

#endif // _EMIR_TARGET_HPP_