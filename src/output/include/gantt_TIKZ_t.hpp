#ifndef _EMIR_GANTT_TIKZ_HPP_
#define _EMIR_GANTT_TIKZ_HPP_

#include <output/OPS_output_t.hpp>
#include <output/OPS_tikz.hpp>
#include <target_set_t.hpp>

namespace emir {

class gantt_TIKZ_t {
 private:
  const TargetSet &T_;
  const OpsInput &input;

  GOMA::matrix<double> D_;

  const double gantt_w_;

  const double task_h_;
  const double slit_gap_;
  const double task_gap_;
  const double axis_x_gap_;
  const double axis_y_gap_;

 public:
  gantt_TIKZ_t(const TargetSet &T, const OpsInput &input) :
    T_(T), input(input), D_(), gantt_w_(30), task_h_(1.0), slit_gap_(0.1),
    task_gap_(0.2), axis_x_gap_(0.1), axis_y_gap_(0.1) {
    T.get_time_matrix(T.get_CSU_setup().b_spd(), D_);
  }

  ~gantt_TIKZ_t() {}

  std::ostream &write(std::ostream &os, const OpsOutput &O) const {
    OPS_TIKZ TIKZ_w;

    os << TIKZ_w.TIKZ_HEADER << '\n';
    os << TIKZ_w.TIKZ_GANTT_HEADER << '\n';

    os << TIKZ_w.TIKZ_OPENING << '\n';

    ////////////////////////////////

    os << TIKZ_w.TIKZ_TEX_BOX_STY << '\n';
    os << TIKZ_w.TIKZ_TEX_BOX_HDR << '\n';

    full_description(os, O);

    os << TIKZ_w.TIKZ_TEX_BOX_FT << '\n';

    os << TIKZ_w.TIKZ_TEX_BOX_TTL << '\n';

    name(os, O);

    os << TIKZ_w.TIKZ_FIG_CLOSING << '\n';

    os << TIKZ_w.TIKZ_END_MINI << '\n';

    ///////////////////////////////

    os << TIKZ_w.TIKZ_FIG_HEADER << '\n';

    os << '\n';

    setup(os, O);

    os << '\n';

    axis(os, O);

    os << TIKZ_w.TIKZ_FIG_CLOSING << '\n';
    os << TIKZ_w.TIKZ_CLOSING << '\n';

    return os;
  }

  std::ostream &full_description(std::ostream &os, const OpsOutput &O) const {
    os << "\t \t \\begin{tabular}{ llcr }" << '\n';
    os << "\\\\" << '\n';
    os << "\t \t  $|V^*|$ & \\hspace{3.3cm} &  &" << std::setw(8)
       << O.n_customers() << " \\\\" << '\n';
    os << "\t \t  max length                &  &  &" << std::setw(8)
       << O.length() << " \\\\" << '\n';
    os << "\t \t  Obj             &  &  &" << std::setw(8) << std::fixed
       << std::setprecision(2) << O.get_obj() << " \\\\" << '\n';
    // os << "\t \t  CSU$_h$            &  &  &" << std::setw(6) << std::fixed
    // << std::setprecision(2) << CSU_st.get_CSU_h() << " \\\\" << '\n'; os <<
    // "\t \t  $h$                &  &  &" << std::setw(6) << std::fixed <<
    // std::setprecision(2) << CSU_st.get_h() << " \\\\" << '\n'; os << "\t \t
    // $d_1$               & &  &" << std::setw(6) << std::fixed <<
    // std::setprecision(2) << CSU_st.get_d1() << " \\\\" << '\n'; os << "\t \t
    // $d_2$               & &  &" << std::setw(6) << std::fixed <<
    // std::setprecision(2) << CSU_st.get_d2() << " \\\\" << '\n';
    os << "\t \t \\end{tabular}" << '\n';
    os << '\n';
    return os;
  }

  std::ostream &name(std::ostream &os, const OpsOutput &O) const {
    std::string name = input.get_instance().get_instance_name();

    const std::string bar = "_";

    size_t pos = 0;

    do {
      pos = name.find(bar, pos);

      if (pos != std::string::npos) {
        name.replace(pos, 1, "\\_");

        pos++;
        pos++;
      }

    } while (pos != std::string::npos);

    const std::string stam = input.get_instance().get_instance_stamp();

    os << "{" << name << "};" << '\n';
    return os;
  }

  void setup(std::ostream &os, const OpsOutput &O) const {
    const int n = input.getN();
    const int K = input.getM();

    std::vector<int> Jk;

    for (int k = 0; k < K; k++) {
      O.get_Jk(Jk, k);

      if (Jk.size() > 0) {
        int current = 0;
        int next = O.get_next(current, k);

        g_setup(os, O, k, current, next);

        current = next;

        while (current != n - 1) {
          next = O.get_next(current, k);

          g_setup(os, O, k, current, next);

          current = next;
        }
      }
    }
  }

  void
  g_setup(std::ostream &os, const OpsOutput &O, int k, int i, int j) const {
    const int n = input.getN();
    const int K = input.getM();

    const double rL = (double)(input.getL()) / input.getScalingFactor();

    const double pi = (i == 0) ? 0 : T_[i - 1].get_p();
    const double pj = (j == (n - 1)) ? 0 : T_[j - 1].get_p();
    const double si = O.s_[i];
    const double sj = O.s_[j];
    const double hj = O.slack(j);
    const double dij = D_(i + 1, j + 1);

    const double x_scal = gantt_w_ / (rL + axis_x_gap_);

    // const double gap1 =  O.s_[j] - O.s_[i];
    // const double gap2 =  (trunc((d  + p)* 10))/10;

    // assert(gap1 >= gap2 - 0.01);

    const double x1 = (si + pi) * x_scal;
    const double x2 = x1 + dij * x_scal;
    const double x3 = sj * x_scal;
    const double x4 = x3 + pj * x_scal;
    const double x5 = x3 + hj * x_scal;
    const double y1 = (k - K + 1) * task_h_ - (task_h_ - 3 * slit_gap_) * 0.5;
    const double y2 = y1 + 0.25;
    // const double y2  = y + 0.25;

    nosty(os, x1, y1, 0);
    nosty(os, x2, y1, 1);
    nosty(os, x3, y1, 2);
    nosty(os, x4, y1, 3);

    nosty(os, x3, y2, 4);
    nosty(os, x5, y2, 5);

    // nosty(os, (x2 + x3) / 2, y, 4);

    os << "\\draw[slit, dotted, >=latex] (0.center) to (1.center);" << '\n';

    if (j != n - 1) {
      os << "\\draw[slit, |-|, blue, >=latex] (2.center) -- (3.center) ";
      os << "node[pos = 0.5, above] {\\scriptsize $j_{" << j << "}$};" << '\n';
    }
  }

  void nosty(std::ostream &os, double x, double y, int id) const {
    os << "\\node [nosty] (";
    os << id;
    os << ") at (" << x << ", " << y << ") {};" << '\n';
  }

  void legend(std::ostream &os, double x, double y, const std::string s) const {
    os << "\\node (";
    os << s;
    os << ") at (" << x << ", " << y << ") {\\scriptsize $" << s << "$};"
       << '\n';
  }

  void axis(std::ostream &os, const OpsOutput &O) const {
    const int K = input.getM();
    const int n = input.getN();

    const double rL = (double)(input.getL()) / input.getScalingFactor();

    const double x = gantt_w_ + 0.5;
    const double y = -K * task_h_ + slit_gap_ - task_gap_;

    os << '\n';
    nosty(os, 0 + 0.02, y, 0);
    nosty(os, 0 + 0.02, 0 + task_gap_, 1);
    nosty(os, x, y, 2);

    os << '\n';

    os << "\\draw[slit] (1.center) to (0.center);" << '\n';
    os << "\\draw[slit, ->, >=latex] (0.center) to (2.center);" << '\n';

    for (int k = 0; k < K; k++) {
      const double yk = (k - K + 1) * task_h_ - (task_h_ - slit_gap_) * 0.5;
      const double xk = -0.5;
      const std::string s = to_std::string(k + 1);
      legend(os, xk, yk, s);
    }

    const double x_scal = gantt_w_ / (rL + axis_x_gap_);

    /*for (int i = 1; i < n - 1; i++) {
        const double si = O.s_[i];
        const double xs = si * x_scal + 0.1;
        const double ys = y - 0.3;
        const std::string s  = "s_{" + to_std::string(i) + "}";

        if (si > 0.01)
            legend(os, xs, ys, s);
    }*/

    {
      const double si = 0 + 0.15;
      const double xs = si;
      const double ys = y - 0.3;
      const std::string s = "0";
      legend(os, xs, ys, s);
    }

    os << '\n';

    {
      const double si = O.s_[n - 1];
      const double xs = si * x_scal + 0.1;
      const double ys = y - 0.3;
      const std::string s = "s_{ n + 1 }";
      legend(os, xs, ys, s);
    }

    os << '\n';

    {
      const double si = rL;
      const double xs = si * x_scal + 0.1;
      const double ys = y - 0.3;
      const std::string s = "L";
      legend(os, xs, ys, s);
    }

    os << '\n';

    /*
    for (int i = 1; i < n - 1; i++) {

        const double si = O.s_[i];
        const double xi = si * x_scal;

        const double sj = O.s_[i + 1];
        const double xj = sj * x_scal;

        if (si > 0.01) {
            nosty(os, xi, y, 1);
            nosty(os, xj, y, 2);

            os << "\\draw[slit, |-, >=latex] (1.center) -- (2.center);" << '\n';
        }
    } */
    {
      const double si = O.s_[n - 1];
      const double xi = si * x_scal;

      const double sj = 0;
      const double xj = sj * x_scal;

      if (si > 0.01) {
        nosty(os, xi, y, 1);
        nosty(os, xj, y, 2);

        os << "\\draw[slit, |-|, >=latex] (1.center) -- (2.center);" << '\n';
      }
    }

    os << '\n';

    {
      const double si = rL;
      const double xi = si * x_scal;

      const double sj = 0;
      const double xj = sj * x_scal;

      if (si > 0.01) {
        nosty(os, xi, y, 1);
        nosty(os, xj, y, 2);

        os << "\\draw[slit, |-, >=latex] (1.center) -- (2.center);" << '\n';
      }
    }

    os << '\n';
  }
};

}  // namespace emir

#endif  // _EMIR_GANTT_TIKZ_HPP_