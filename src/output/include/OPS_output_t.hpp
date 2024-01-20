#pragma once

#include "OPS_input_t.hpp"
#include <vector>
#include "matrix.hpp"


using namespace std;


namespace EMIR
{



class OPS_output_t
{
public:
    const OPS_input_t& I_;

    GOMA::matrix<int>      x_;
    vector<int>      y_;
    vector<double>   s_;
    vector<double>   h_;

    GOMA::matrix<int>      t_cost_;

    bool optimal_;
    bool found_;

public:
    OPS_output_t(const OPS_input_t& I);
    
    OPS_output_t(const OPS_output_t& O);
    
    virtual ~OPS_output_t(void);

    virtual int get_n() const
    {
        return I_.get_n();
    }
    virtual int get_m() const
    {
        return I_.get_m();
    }

    bool set(const vector<double>& x, const vector<double>& y, const vector<double>& s, bool optimal = true);
    bool set(const vector<double>& x, const vector<double>& y, bool optimal = true);
	void set(vector<double>& x, vector<int>& y);

    int  get_x(int k, int i, int j) const;
    virtual int& set_x(int k, int i, int j);
    virtual int& set_x(GOMA::matrix<int>& M, int k, int i, int j) const;
    int  get_y(int j) const;
    virtual int& set_y(int i);
    double  get_s(int j) const;

    void get_Jk(vector<int>& Jk, int k) const;

    int get_obj(void) const;
    double length(int k) const;
    double length(void) const;

    double slack(int i) const;
    double slack(void) const;

    bool found(void) const
    {
        return found_;
    }
    bool optimal(void) const
    {
        return optimal_;
    }

    int n_customers(void) const;

    const OPS_input_t& get_input(void) const
    {
        return I_;
    }

    ostream& write(ostream& os) const;

    void write_statistics(ostream& os) const;

    int get_next(int i, int k) const;
    int get_prev(int i, int k) const;

    void init_t_cost(void);

    void set(const OPS_output_t& O, int k);    
    void set(const OPS_output_t& O);
	void setADD(const OPS_output_t& O);

    bool idle(int k) const
    {
        const int n = get_n();
        return (x_(1 + k * n, n) == 1);
    }

    bool check(void);

protected:

};


}
