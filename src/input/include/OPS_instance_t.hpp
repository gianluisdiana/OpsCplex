#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "matrix.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

#define N_ITEM 2

#define INF_SPP 999999

namespace EMIR
{

    enum
    {
        NAME,
        STAMP
    };

    /**
     * @class OPS_instance_t
     * @author Jorge Riera-Ledesma
     * @date 06/12/17
     * @file OPS_instance_t.hpp
     * @brief Class describing an instance for the OPS
     */

    class OPS_instance_t
    {
    private:
        vector<string> id_; /**< Id of the instance Id of the source target points @see target_set_t.hpp */

        int type_; /**< Type of instance generation @see instance_code_type.hpp */

        vector<vector<int>> Jk_; /**< Assignment of jobs to processors \f$ J_k \f$ */
        vector<vector<int>> Kj_; /**< Assignment of processors to jobs \f$ K_j \f$ */
        GOMA::matrix<int> T_;    /**< Cost matrix \f$ T \f$ */
        vector<int> b_;          /**< Profit for each job \f$ b \f$ */

        double alpha_; /**< Percentage of the total tour */
        int L_;        /**< Time limit  \f$ L \f$ */

        double scal_factor_;

    public:
        OPS_instance_t(void);
        virtual ~OPS_instance_t(void);

        istream &read(istream &is);
        ostream &write(ostream &os) const;

        inline const string &get_instance_name(void) const
        {
            return id_[NAME];
        }
        inline const string &get_instance_stamp(void) const
        {
            return id_[STAMP];
        }

        inline int get_n(void) const
        {
            return b_.size();
        }
        inline int get_m(void) const
        {
            return Jk_.size();
        }
        inline const vector<int> &get_Jk(int k) const
        {
            return Jk_[k];
        }
        inline int get_b(int j) const
        {
            return b_[j];
        }
        inline int get_L(void) const
        {
            return L_;
        }
        inline const GOMA::matrix<int> &get_T(void) const
        {
            return T_;
        }
        inline double get_scal_factor(void) const
        {
            return scal_factor_;
        }

        int get_max_Jk(void) const;

        const vector<vector<int>> &get_Kj(void) const { return Kj_; }

        void set(const string &source_name, const string &source_stamp, const string &desc, int type, const vector<vector<int>> &Jk, const GOMA::matrix<int> &T, const vector<int> &b);
        void set(const OPS_instance_t &O);
        void set(const string &name, const string &stamp, const string &desc);
        void set_L(double alpha, int L);

        void write_statistics(ostream &os) const;
        void write_statistics_hdr(ostream &os) const;

    private:
        void get_json(json &instance) const;
        void set_json(const json &instance);

        void make_Kj(void);
    };
}

istream &operator>>(istream &is, EMIR::OPS_instance_t &input);
ostream &operator<<(ostream &os, const EMIR::OPS_instance_t &input);
