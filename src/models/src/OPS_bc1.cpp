#include "OPS_bc1.hpp"

namespace EMIR
{

    OPS_cplex_solver1::OPS_cplex_solver1(const OPS_input_t *I, OPS_output_t &O, double eps) : OPS_solver_t(I, O, eps),
                                                                                                                                                   env_(),
                                                                                                                                                   x_(env_),
                                                                                                                                                   y_(env_),
                                                                                                                                                   s_(env_),
                                                                                                                                                   cplex_(env_)
    {
    }

    OPS_cplex_solver1::~OPS_cplex_solver1(void)
    {
        env_.end();
    }

    void OPS_cplex_solver1::set_param(ostream &r_os)
    {

        cplex_.setParam(IloCplex::Param::TimeLimit, 3600);

        cplex_.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, 1E-3);
        cplex_.setParam(IloCplex::Param::Emphasis::MIP, CPX_MIPEMPHASIS_OPTIMALITY);

        cplex_.setOut(r_os);
    }

    void OPS_cplex_solver1::solve(ostream &r_os, double ub, bool root_node)
    {

        try
        {

            IloModel model(env_);
            makeModel(model);

            set_param(r_os);

            cplex_.extract(model);

            cplex_.solve();
        }
        catch (IloException &ex)
        {
            cerr << "Error: " << ex << endl;
            return;
        }
        catch (...)
        {
            cerr << "Error" << endl;
            return;
        }

        set_output(O_);
    }

    void OPS_cplex_solver1::set_output(OPS_output_t &output)
    {
        IloNumArray x(env_);
        cplex_.getValues(x_, x);

        IloNumArray y(env_);
        cplex_.getValues(y_, y);

        IloNumArray s(env_);
        cplex_.getValues(s_, s);

        vector<double> xv(x.getSize());

        for (int i = x.getSize() - 1; i >= 0; i--)
            xv[i] = x[i];

        vector<double> yv(y.getSize());

        for (int i = y.getSize() - 1; i >= 0; i--)
            yv[i] = y[i];

        vector<double> sv(s.getSize());

        sv[0] = s[0];
        sv[s.getSize() - 1] = s[s.getSize() - 1];

        for (int i = s.getSize() - 2; i > 0; i--)
            sv[i] = s[i] * y[i - 1];

        output.set(xv, yv, sv);
    }

    void OPS_cplex_solver1::makeModel(IloModel &model)
    {

        const int n = I_->get_n(); // Number of vertices plus two depots
        const int K = I_->get_m(); // Number of vehicles
        const int L = I_->get_L(); // Limit

        const int max_arc = I_->get_max_arc() + 1;
        const int big_m = max_arc > L ? max_arc : L;

        char aux[80];

        int m = 0;

        for (int k = 0; k < K; k++)
            m += I_->get_msucc(k);

        // Variables

        // n - 2 variables y

        for (int j = 1; j < n - 1; j++)
        {

            sprintf(aux, "y_%d", j);
            y_.add(IloNumVar(env_, 0, 1, IloNumVar::Bool, aux));
        }

        model.add(y_);

        // n - 1 variables s
        // n - 1 variables s

        for (int j = 0; j < n; j++)
        {

            sprintf(aux, "s_%d", j);
            s_.add(IloNumVar(env_, 0, IloInfinity, IloNumVar::Float, aux));
        }

        model.add(s_);

        // Variables x

        const int sz = I_->get_A_succ_sz();

        for (int l = 0; l < sz; l++)
        {

            const int pos = I_->get_A_succ(l);

            int i, j, k;
            I_->get_pos(pos, k, i, j);

            sprintf(aux, "x_%d_%d_%d", k + 1, i, j);
            x_.add(IloNumVar(env_, 0, 1, IloNumVar::Bool, aux));
        }

        model.add(x_);

        // Objective

        IloExpr obj(env_);

        for (int i = 1; i < n - 1; i++)
            obj += I_->get_b(i) * y_[i - 1];

        model.add(IloMaximize(env_, obj));

        obj.end();

        // Constraints

        IloRangeArray constraints(env_);

        for (int k = 0; k < K; k++)
        {

            const int mki = I_->get_nsucc(k, 0);

            if (mki != 0)
            {

                IloExpr cut(env_);

                for (int l = 0; l < mki; l++)
                    cut += x_[I_->get_succ_inx(k, 0, l)];

                sprintf(aux, "deltaplus_%d_%d", 0, k + 1);
                constraints.add(IloRange(env_, 1.0, cut, 1.0, aux));
                cut.end();
            }
        }

        for (int k = 0; k < K; k++)
        {

            const int mki = I_->get_npred(k, n - 1);

            if (mki != 0)
            {

                IloExpr cut(env_);

                for (int l = 0; l < mki; l++)
                    cut += x_[I_->get_pred_inx(k, n - 1, l)];

                sprintf(aux, "deltaminus_%d_%d", n - 1, k + 1);
                constraints.add(IloRange(env_, 1.0, cut, 1.0, aux));
                cut.end();
            }
        }

        // DELTA PLUS

        for (int k = 0; k < K; k++)
        {

            for (int i = 1; i < n - 1; i++)
            {

                const int mki = I_->get_nsucc(k, i);

                if (mki != 0)
                {

                    IloExpr cut(env_);

                    for (int l = 0; l < mki; l++)
                        cut += x_[I_->get_succ_inx(k, i, l)];

                    cut -= y_[i - 1];

                    sprintf(aux, "deltaplus_%d_%d", i, k + 1);
                    constraints.add(IloRange(env_, 0.0, cut, 0.0, aux));
                    cut.end();
                }
            }
        }

        // DELTA MINUS

        for (int k = 0; k < K; k++)
        {

            for (int i = 1; i < n - 1; i++)
            {

                const int mki = I_->get_npred(k, i);

                if (mki != 0)
                {

                    IloExpr cut(env_);

                    for (int l = 0; l < mki; l++)
                        cut += x_[I_->get_pred_inx(k, i, l)];

                    cut -= y_[i - 1];

                    sprintf(aux, "deltaminus_%d_%d", i, k + 1);
                    constraints.add(IloRange(env_, 0.0, cut, 0.0, aux));
                    cut.end();
                }
            }
        }

        // MTZ

        for (int l = 0; l < sz; l++)
        {

            int i;
            int j;
            int k;

            const int arc = I_->get_A_succ(l);

            I_->get_pos(arc, k, i, j);

            IloExpr cut(env_);

            cut = big_m * x_[l] + s_[i] - s_[j];

            sprintf(aux, "MTZ_%d_%d_%d", i, j, k + 1);

            // cout << "( "<< i << ", " << j << " ): " << I_->get_t(i,j) << endl;

            constraints.add(IloRange(env_, -IloInfinity, cut, big_m - I_->get_t(i, j), aux));
            cut.end();
        }

        // LIMIT

        {
            IloExpr cut(env_);

            cut = s_[0];

            sprintf(aux, "Limit0");
            constraints.add(IloRange(env_, 0, cut, 0, aux));
            cut.end();
        }

        {
            IloExpr cut(env_);

            cut = s_[n - 1];

            sprintf(aux, "Limit");
            constraints.add(IloRange(env_, -IloInfinity, cut, L, aux));
            cut.end();
        }

        model.add(constraints);
        constraints.end();

        // cout << model << endl;
    }

}