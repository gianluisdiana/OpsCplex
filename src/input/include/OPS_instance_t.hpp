#ifndef _EMIR_OPS_INSTANCE_HPP_
#define _EMIR_OPS_INSTANCE_HPP_

#include <vector>
#include <iostream>
#include <string>

#include "matrix.hpp"
#include "json.hpp"

using json = nlohmann::json;

#define N_ITEM 2

#define INF_SPP 999999

namespace emir {

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
        std::vector<std::string> id_; /**< Id of the instance Id of the source target points @see target_set_t.hpp */

        int type_; /**< Type of instance generation @see instance_code_type.hpp */

        std::vector<std::vector<int>> Jk_; /**< Assignment of jobs to processors \f$ J_k \f$ */
        std::vector<std::vector<int>> Kj_; /**< Assignment of processors to jobs \f$ K_j \f$ */
        GOMA::matrix<int> T_;    /**< Cost matrix \f$ T \f$ */
        std::vector<int> b_;          /**< Profit for each job \f$ b \f$ */

        double alpha_; /**< Percentage of the total tour */
        int L_;        /**< Time limit  \f$ L \f$ */

        double scal_factor_;

    public:
        OPS_instance_t(void);
        virtual ~OPS_instance_t(void);

        std::istream &read(std::istream &is);
        std::ostream &write(std::ostream &os) const;

        inline const std::string &get_instance_name(void) const
        {
            return id_[NAME];
        }
        inline const std::string &get_instance_stamp(void) const
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
        inline const std::vector<int> &get_Jk(int k) const
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

        const std::vector<std::vector<int>> &get_Kj(void) const { return Kj_; }

        void set(const std::string &source_name, const std::string &source_stamp, const std::string &desc, int type, const std::vector<std::vector<int>> &Jk, const GOMA::matrix<int> &T, const std::vector<int> &b);
        void set(const OPS_instance_t &O);
        void set(const std::string &name, const std::string &stamp, const std::string &desc);
        void set_L(double alpha, int L);

        void write_statistics(std::ostream &os) const;
        void write_statistics_hdr(std::ostream &os) const;

    private:
        void get_json(json &instance) const;
        void set_json(const json &instance);

        void make_Kj(void);
    };

} // namespace emir

std::istream &operator>>(std::istream &is, emir::OPS_instance_t &input);
std::ostream &operator<<(std::ostream &os, const emir::OPS_instance_t &input);

#endif // _EMIR_OPS_INSTANCE_HPP_