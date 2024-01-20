#include "OPS_output_t.hpp"
#include <cassert>
#include <cmath>

#include <iomanip>

#include <set>

#define O_EPS 1E-2

namespace EMIR
{

OPS_output_t::OPS_output_t(const OPS_input_t& I)
  : I_(I)
  , x_(get_n() * get_m(), get_n())
  , y_(get_n())
  , s_(get_n())
  , h_(get_n())
  , t_cost_(get_n(), get_n())
  , optimal_(false)
  , found_(false)
{

    x_.init(0);

    const int n = get_n();
    const int K = get_m();

    for(int k = 0; k < K; k++)
	x_(1 + k * n, n) = 1;

    const int sz_y = y_.size();

    for(int i = 0; i < sz_y; i++)
	y_[i] = 0;

    const int sz_s = s_.size();

    for(int i = 0; i < sz_s; i++)
	s_[i] = 0;

    const int sz_h = h_.size();

    const int L = I.get_L();
    const double scal_factor = I.get_scal_factor();

    for(int i = 0; i < sz_h; i++)
	h_[i] = L / scal_factor;

    init_t_cost();

    check();
}

OPS_output_t::OPS_output_t(const OPS_output_t& O)
  : I_(O.I_)
  , x_(O.x_)
  , y_(O.y_)
  , s_(O.s_)
  , h_(O.h_)
  , t_cost_(O.t_cost_)
  , optimal_(O.optimal_)
  , found_(O.found_)
{
    check();
}

OPS_output_t::~OPS_output_t(void)
{
}

void
OPS_output_t::set(const OPS_output_t& O)
{
    const int K = get_m();
    const int n = get_n();

    x_.init(0);

    for(int i = 0; i < n; i++)
	{
	    s_[i] = 0;
	    h_[i] = 0;
	}

    for(int k = 0; k < K; k++)
	{
	    if(!O.idle(k))
		{
		    set(O, k);
		}
	    else
			x_(k * n + 1, n) = 1;
	}

	vector<double> x;

    set(x, y_);

	if (s_[n-1] < I_.get_L())
		found_ = false;

    check();
}

void
OPS_output_t::setADD(const OPS_output_t& O)
{
    const int K = get_m();
    const int n = get_n();

    for(int i = 0; i < n; i++)
	{
	    s_[i] = 0;
	    h_[i] = 0;
	}

    for(int k = 0; k < K; k++){
	if(!O.idle(k))
	    {
		set(O, k);
	    }
	}

	vector<double> x;

    set(x, y_);

	
	const double L = I_.get_L() / I_.get_scal_factor();

	if (s_[n-1] <=  L)
		found_ = true;
	else
		found_ = false;
		
    check();
}

void
OPS_output_t::set(vector<double>& x, vector<int>& y)
{
    const int mz = I_.get_A_succ_sz();
    x.resize(mz);

    for(int i = 0; i < mz; i++)
	x[i] = 0;

    const int n = get_n();
	
	y.resize(n);
	for(int i = 0; i < n; i++)
		y[i] = 0;
	y[0] = 1;
	y[n - 1] = 1;
	
    const int m = get_m();

    for(int k = 0; k < m; k++)
	{
	    for(int i = 1; i <= n; i++)
		{
		    for(int j = 1; j <= n; j++)
			{
			    const int val = x_(k * n + i, j);

			    if(val == 1)
				{
				    y_[i - 1] = 1;
				    y_[j - 1] = 1;

				    const int pos = I_.get_inv_succ(k, i - 1, j - 1);
				    x[pos] = val;
				}
			}
		}
	}
}

void
OPS_output_t::set(const OPS_output_t& O, int k)
{
    const int n = get_n();

    std::set<int> S;

    for(int i = 1; i <= n; i++)
	{
	    for(int j = 1; j <= n; j++)
		{
		    x_(k * n + i, j) = O.x_(k * n + i, j);
		}
	}
}

void
OPS_output_t::init_t_cost(void)
{
    const int n = get_n();

    for(int i = 1; i <= n - 1; i++)
	{

	    for(int j = 2; j <= n; j++)
		if((i != j) && !((i == 1) && (j == n)))
		    {

			t_cost_(i, j) = I_.instance_.get_T()(i, j);
		    }
		else
		    t_cost_(i, j) = INF_SP;
	}

    t_cost_(1, n) = 0;

    for(int j = 1; j <= n - 1; j++)
	t_cost_(n, j) = INF_SP;

    for(int i = 1; i <= n; i++)
	t_cost_(i, 1) = INF_SP;
}

int
OPS_output_t::get_next(int i, int k) const
{
    const int n = get_n();
    const int i_pos = i + 1 + k * n;

    for(int j = 1; j <= n; j++)
	if(x_(i_pos, j) > 0)
	    return j - 1;

    assert(false);
    return -1;
}

int
OPS_output_t::get_prev(int i, int k) const
{
    const int n = get_n();
    const int i_pos = i + 1;

    for(int j = 1; j <= n; j++)
	if(x_(j + k * n, i_pos) > 0)
	    return j - 1;

    assert(false);
    return -1;
}

double
OPS_output_t::slack(int i) const
{
    return h_[i] - s_[i];
}

double
OPS_output_t::slack(void) const
{
    const int n = x_.get_n() - 1;

    return h_[n] - s_[n];
}

bool
OPS_output_t::set(const vector<double>& x, const vector<double>& y, const vector<double>& s, bool optimal)
{

    found_ = true;
    optimal_ = optimal;

    x_.init(0);

    const int n = get_n();

    const int x_sz = x.size();

    for(int l = 0; l < x_sz; l++)
	{
	    const int arc = I_.get_A_succ(l);

	    int i;
	    int j;
	    int k;

	    I_.get_pos(arc, k, i, j);

	    const int val = round(x[l]);

	    assert(val <= 1);
	    assert(val >= 0);

	    if(val == 1)
		set_x(k, i, j) = 1;
	}

    y_[0] = 1;
    y_[n - 1] = 1;

    if(y.size() > 0)
	{

	    for(int i = 1; i < n - 1; i++)
		{

		    const int val = round(y[i - 1]);

		    assert(val <= 1);
		    assert(val >= 0);

		    y_[i] = val;
		}
	}

    s_[0] = 0;

    if(s.size() > 0)
	{

	    for(int i = 1; i < n; i++)
		{

		    const int val = s[i - 1];

		    assert(val >= 0);

		    s_[i] = val;
		}
	}

    for(int i = 0; i < n; i++)
	s_[i] /= I_.get_scal_factor();

    // x_.write_raw(cout);

    check();

    return found_;
}

void
OPS_output_t::get_Jk(vector<int>& Jk, int k) const
{
    Jk.clear();

    const vector<int>& J = I_.get_Jk(k);

    for(int j : J) {
	if(y_[j] > 0)
	    Jk.push_back(j);
	}
}

bool
OPS_output_t::set(const vector<double>& x, const vector<double>& y, bool optimal)
{

    found_ = true;
    optimal_ = optimal;

    x_.init(0);

    const int n = get_n();
    const int K = get_m();
    const int In = I_.get_n();

    GOMA::matrix<int> new_x(n * K, n);
    new_x.init(0);

    for(int k = 0; k < K; k++)
	{
	    x_(1 + k * n, n) = 1;
	}

    const int x_sz = x.size();

    for(int l = 0; l < x_sz; l++)
	{

	    const int arc = I_.get_A_succ(l);

	    int i;
	    int j;
	    int k;

	    I_.get_pos(arc, k, i, j);

	    const int val = round(x[l]);

	    assert(val <= 1);
	    assert(val >= 0);

	    if(val == 1)
		{
		    set_x(k, 0, In - 1) = 0;
		    set_x(k, i, j) = 1;
		}
	}

    y_[0] = 1;
    y_[n - 1] = 1;

    for(int i = 1; i < n - 2; i++)
	y_[i] = 0;

    if(y.size() > 0)
	{

	    for(int i = 1; i < In - 1; i++)
		{

		    const int val = round(y[i - 1]);

		    assert(val <= 1);
		    assert(val >= 0);

		    set_y(i) = val;
		}
	}

    return check();
}



void
OPS_output_t::write_statistics(ostream& os) const
{
    os << setw(4) << n_customers() << "\t";

    if(found_)
	os << setw(10) << get_obj() << "\t";
    else
	os << setw(10) << 99999999 << "\t";

    if(found_)
	os << setw(10) << fixed << setprecision(1) << length() * 10 << "\t";
    else
	os << setw(10) << 0 << "\t";

    if(found_)
	os << setw(4) << 1 << "\t";
    else
	os << setw(4) << 0 << "\t";

    if(optimal_)
	os << setw(4) << 1 << "\t";
    else
	os << setw(4) << 0 << "\t";
}

ostream&
OPS_output_t::write(ostream& os) const
{
    /* x_.write_raw(os);

    os << endl;*/
    vector<int> r;
    I_.get_r(r);

    const int n = get_n();

    /*for(int i = 0; i < n; i++)
        os << setw(5) << y_[i];

    os << endl;
    os << endl; */

    for(int i = 0; i < n; i++)
	if(fabs(s_[i] > 0.0001))
	    os << setw(4) << i << " " << setw(9) << fixed << setprecision(2) << s_[i] << " " << setw(9) << fixed
	       << setprecision(2) << h_[i] << " " << endl;

    os << endl;
    os << endl;

    return os;
}

int
OPS_output_t::get_x(int k, int i, int j) const
{
    return x_(i + 1 + k * get_n(), j + 1);
}

int&
OPS_output_t::set_x(int k, int i, int j)
{
    return x_(i + 1 + k * get_n(), j + 1);
}

int&
OPS_output_t::set_x(GOMA::matrix<int>& M, int k, int i, int j) const
{
    return M(i + 1 + k * get_n(), j + 1);
}

int&
OPS_output_t::set_y(int j)
{
    return y_[j];
}

int
OPS_output_t::get_y(int j) const
{
    return y_[j];
}

double
OPS_output_t::get_s(int j) const
{
    return s_[j];
}

int
OPS_output_t::get_obj(void) const
{
    assert((s_.size() > 0) && (s_[0] != -1));

    int obj = 0;

    for(int j = y_.size() - 1; j >= 0; j--)
	if(y_[j] > 0)
	    {

		const double b = I_.OPS_input_t::get_b(j);
		obj += y_[j] * b;
	    }

    return obj;
}

int
OPS_output_t::n_customers(void) const
{
    int cnt = 0;
    const int nc = y_.size() - 1;

    for(int i = 1; i < nc; i++)
	if(y_[i] > 0)
	    cnt++;

    return cnt;
}

double
OPS_output_t::length(void) const
{
    return s_[y_.size() - 1];
}

double
OPS_output_t::length(int k) const
{
    assert((s_.size() > 0) && (s_[0] != -1));

    double len = 0;
    const int n = get_n();

    for(int i = 0; i < n; i++)
	for(int j = 0; j < n; j++)
	    len += get_x(k, i, j);

    return len;
}

bool
OPS_output_t::check(void)
{
    found_ = true;

    const int n = get_n();
    const int K = get_m();
    const int L = I_.get_L();

    vector<int> i_degree(n);
    vector<int> o_degree(n);

    for(int i = 0; i < n; i++)
	{
	    i_degree[i] = 0;
	    o_degree[i] = 0;
	}

    for(int k = 0; k < K; k++)
	for(int i = 0; i < n; i++)
	    for(int j = 0; j < n; j++)
		{

		    if(get_x(k, i, j) == 1)
			{

			    i_degree[j]++;
			    o_degree[i]++;
			}
		}

    assert(o_degree[0] == K);
    assert(i_degree[n - 1] == K);

    for(int i = 1; i < n - 1; i++)
	assert((o_degree[i] - i_degree[i]) == 0);

    for(int i = 1; i < n - 1; i++)
	if(i_degree[i] > 0)
	    assert(y_[i] == 1);
	else
	    assert(y_[i] == 0);

    const double rL = (double)(L) / I_.get_scal_factor();

    /*for(int j = 0; j < n; j++)
                    cout << "Nodo: " << setw(3) << j << ": " << setw(8) << fixed << setprecision(1) << s_[j]
                         << " -> " << setw(8) << fixed << setprecision(1) << rL << endl;
    cout << endl; */

    for(int i = 0; i < n; i++)
	{
	    if(s_[i] > rL + O_EPS)
		{

		    found_ = false;

		    cout << "Nodo: " << i << ": " << s_[i] << " -> " << rL << endl;

		    for(int j = 0; j < n; j++)
			cout << "Nodo: " << setw(3) << j << ": " << setw(8) << fixed << setprecision(1) << s_[j]
			     << " -> " << setw(8) << fixed << setprecision(1) << rL << endl;
		    cout << endl;

		    assert(s_[i] <= rL + O_EPS);
		    exit(1);
		}
	}

    /*cout << endl;

    for(int j = 0; j < n; j ++)
       cout << "Nodo: "<< setw(3)<< j << ": " << setw(8) << fixed << setprecision(1) <<  s_[j] << " -> " << setw(8) <<
    fixed << setprecision(1) << rL << endl; cout << endl;     */

    return found_;
}
}
