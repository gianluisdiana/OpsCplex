#include "OPS_instance_t.hpp"

#include <iomanip>
#include <cstdio>
#include <cassert>


namespace emir {


OPS_instance_t::OPS_instance_t(void):
    id_(N_ITEM),
    type_(),
    Jk_(),
    Kj_(),
    T_(),
    b_(),
    alpha_(-1),
    L_(0),
    scal_factor_(10)
{
}

OPS_instance_t::~OPS_instance_t(void)
{}

void OPS_instance_t::write_statistics_hdr(std::ostream& os) const
{
    os << "ID" << "\t";
}

void OPS_instance_t::write_statistics(std::ostream& os) const
{
    os << id_[NAME] << "\t";
}


void OPS_instance_t::get_json(json& instance) const
{
    json jT;
    T_.get_json(jT);

    instance["id"]       = id_;
    instance["type"]     = type_;

    instance["Jk"]       = Jk_;
    instance["T"]        = jT;
    instance["b"]        = b_;
    instance["alpha"]    = alpha_;
    instance["L"]        = L_;
}

void OPS_instance_t::set_json(const json& instance)
{
    json jT   = instance["T"];

    T_.set_json(jT);

    id_   = instance["id"].get<std::vector<std::string> >();
    type_ = instance["type"].get<int>();
    Jk_   = instance["Jk"].get<std::vector<std::vector<int> > >();
    //b_    = instance["b"].get<std::vector<int> >();
    b_    = instance["b"].get<std::vector<int> >();
    alpha_= instance["alpha"].get<double>();
    L_    = instance["L"].get<int>();        
}

std::istream& OPS_instance_t::read(std::istream& is)
{
    json instance;
    is >> instance;

    set_json(instance);
    
    make_Kj();
	
	const int m = T_.get_m();
	const int n = T_.get_n();
	
	for(int i = 1; i <= m; i++)
		for(int j = 1; j <= n; j++)
			if (T_(i,j) >= L_)
				T_(i,j) = INF_SPP + 1;

    return is;
}

void OPS_instance_t::set(const std::string& name, const std::string& stamp, const std::string& desc)
{
    id_[NAME] = name;
    id_[STAMP]= stamp;
}

int OPS_instance_t::get_max_Jk(void) const
{
    int max = 0;

    for(auto Jk: Jk_)
        if ((int)(Jk.size()) > max)
            max = Jk.size();

    return max;
}

void OPS_instance_t::set_L(double alpha, int L)
{
    alpha_ = alpha;
    L_     = L;
}

void OPS_instance_t::set(const std::string& tar_name, const std::string& tar_stamp, const std::string& tar_desc, int type, const std::vector<std::vector<int> >& Jk, const GOMA::matrix<int>& T, const std::vector<int>& b)
{

    id_[NAME] = tar_name;
    id_[STAMP]= tar_stamp;
	type_ = type;
	

    Jk_           = Jk;
    T_            = T;
    b_            = b;
	
	make_Kj();
}

void OPS_instance_t::set(const OPS_instance_t& O)
{
    id_[NAME] = O.id_[NAME];
    id_[STAMP]= O.id_[STAMP];
	type_ = O.type_;
	

    Jk_           = O.Jk_;
    T_            = O.T_;
    b_            = O.b_;
	
	make_Kj();	
}

void OPS_instance_t::make_Kj(void)
{
    Kj_.clear();

    const int K = get_m();

    Kj_.resize(get_n());

    for(int k = 0; k < K; k++) {

        const std::vector<int>& Jk = get_Jk(k);

        for(int i: Jk)
            Kj_[i].push_back(k);
    }
}

std::ostream& OPS_instance_t::write(std::ostream& os) const
{
    json instance;
    get_json(instance);

    os << std::setw(2) << instance;

    return os;
}

std::istream& operator>>(std::istream& is, OPS_instance_t& I)
{
    I.read(is);
    return is;
}

std::ostream& operator<<(std::ostream& os, const OPS_instance_t& I)
{
    I.write(os);
    return os;
}

}
