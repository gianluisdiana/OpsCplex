#pragma once

#include "OPS_instance_t.hpp"
#include "matrix.hpp"

using namespace std;

#define INF_SP 999999

namespace EMIR
{

    class OPS_input_t
    {

    public:
        const OPS_instance_t &instance_;

        GOMA::matrix<vector<int>> succ_;
        GOMA::matrix<vector<int>> pred_;

        GOMA::matrix<vector<int>> succ_inx_;
        GOMA::matrix<vector<int>> pred_inx_;

        vector<int> A_succ_;

        vector<vector<int>> nodes_k_;

        vector<GOMA::matrix<int>> inv_succ_;

        vector<vector<int>> arcs_k_;

        GOMA::matrix<int> t_cost_; // Matriz de costes transformada

    public:
        OPS_input_t(const OPS_instance_t &instance, bool build = true);

        virtual ~OPS_input_t(void);

        const OPS_instance_t &get_instance(void) const { return instance_; }

        inline virtual int get_n() const { return instance_.get_n(); }
        inline virtual int get_m() const { return instance_.get_m(); }

        inline virtual int get_n_x(void) const { return get_mm(); }
        inline virtual int get_n_y(void) const { return get_n() - 2; }

        inline virtual const vector<int> &get_Jk(int k) const { return instance_.get_Jk(k); }
        inline virtual const vector<vector<int>> &get_Kj(void) const { return instance_.get_Kj(); }
        inline const GOMA::matrix<int> &get_T(void) const { return t_cost_; }
        inline virtual int get_T(int i, int j) const { return instance_.get_T()(i, j); }
        inline int get_t(int i, int j) const { return t_cost_(i + 1, j + 1); }
        inline virtual int get_b(int j) const { return instance_.get_b(j); }
        inline int get_L() const { return instance_.get_L(); }
        virtual void get_L(vector<int> &L) const;
        inline double get_scal_factor() const { return instance_.get_scal_factor(); }

        inline const vector<int> &get_arcs_k(int k) const { return arcs_k_[k]; }

        inline int get_nsucc(int k, int i) const { return succ_(k + 1, i + 1).size(); }
        inline int get_npred(int k, int i) const { return pred_(k + 1, i + 1).size(); }

        int get_msucc(int k) const;

        inline int get_succ_inx(int k, int i, int l) const { return succ_inx_(k + 1, i + 1)[l]; }
        inline const vector<int> &get_succ_inx(int k, int i) const { return succ_inx_(k + 1, i + 1); }
        inline int get_succ(int k, int i, int l) const { return succ_(k + 1, i + 1)[l]; }
        inline int get_pred_inx(int k, int i, int l) const { return pred_inx_(k + 1, i + 1)[l]; }

        inline int get_inv_succ(int k, int i, int j) const
        { /* assert (inv_succ_[k](i + 1, j + 1) != -1); */
            return inv_succ_[k](i + 1, j + 1);
        }
        const vector<int> get_inv_succ(int i, int j) const;

        inline int get_A_succ_sz(void) const { return A_succ_.size(); }
        inline int get_A_succ(int l) const { return A_succ_[l]; }

        int get_pos(int k, int i, int j) const { return (i * get_n() + j + get_n() * get_n() * k); }
        void get_pos(int pos, int &k, int &i, int &j) const;

        int get_mm(void) const;

        const vector<int> &get_nodes(int k) const { return nodes_k_[k]; }

        void get_sync_stat(int &nsync, int &maxgsync, int &mingsync, double &avggsync) const;
        double get_avg_nodes(void) const;
        int get_max_nodes(void) const;
        int get_max_arc(void) const;

        void get_path(const vector<int> &v, int k, vector<int> &arcs) const;
        void get_path(const vector<int> &v, int k, vector<int> &arcs, vector<bool> &visited) const;
        void get_mpath(const vector<int> &v, int k, vector<int> &arcs, vector<bool> &visited) const;

        bool check_path(const vector<int> &p) const;
        int length_path(const vector<int> &p) const;

        void write_statistics(ostream &os) const;
        void write_statistics_hdr(ostream &os) const;

        void write_arc_inx(ostream &os, int inx) const;

        virtual inline int get_mapped_v(int i) const
        {
            return i;
        }

        virtual inline int get_inv_mapped_v(int i) const
        {
            return i;
        }

        virtual void get_mapped_r(vector<int> &r) const
        {
            get_r(r);
        }

        virtual void get_r(vector<int> &r) const
        {

            r.resize(get_n());

            for (int &i : r)
                i = 0;
        }

        virtual void get_d(vector<int> &d) const
        {

            d.resize(get_n());

            for (int &i : d)
                i = get_L();
        }

    protected:
        void test_succ(void);
        void test_pred(void);

        void test_A_succ(void);
        void test_A_pred(void);

        void shortest_path(const GOMA::matrix<int> &M, int source, int *dist, int *prev);

        void resize_structures(void);
        void update_structures(int k, int Ji, int Jj, int &l, GOMA::matrix<int> &A_inv, vector<int> &arcs_k, vector<int> &nodes);
        void update_pred(int k, int Ji, int Jj, int &l);
        void build_input(void);

        void init_t_cost(void);
        void make_structures(void);
        void make_prev(void);
    };

}
