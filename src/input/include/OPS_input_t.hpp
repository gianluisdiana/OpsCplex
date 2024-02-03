/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file OPS_input_t.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.1.1
 * @date January 21, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the graph structure for the
 * O.P.S.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex
 * @endlink
 * @see Selective routing problem with synchronization @link
 * https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1
 * @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link
 * https://google.github.io/styleguide/cppguide.html @endlink
 */

#ifndef _EMIR_OPS_INPUT_HPP_
#define _EMIR_OPS_INPUT_HPP_

#include <OPS_instance_t.hpp>
#include <matrix.hpp>

namespace emir {

class OpsInput : public OpsInstance {
 public:
  // Successors of each node (k, i) with respect to the k-th sliding bar and the
  // i-th object
  GOMA::matrix<std::vector<int>> succ_;
  // Predecessors of each node (k, i) with respect to the k-th sliding bar and
  // the i-th object
  GOMA::matrix<std::vector<int>> pred_;

  // Indexes of the arcs described at succ_ matrix with respect to the k-th
  // sliding bar and the i-th object
  GOMA::matrix<std::vector<int>> succ_inx_;
  // Indexes of the arcs described at pred_ matrix with respect to the k-th
  // sliding bar and the i-th object
  GOMA::matrix<std::vector<int>> pred_inx_;

  // 1D vector that represents a 3D structure with the sliding bars, the objects
  // and its successors
  std::vector<int> A_succ_;
  // Index of the reached objects in the k-th sliding bar
  std::vector<std::vector<int>> nodes_k_;
  std::vector<GOMA::matrix<int>> inv_succ_;
  // Arcs indexes of the k-th sliding bar
  std::vector<std::vector<int>> arcs_k_;

  // Time matrix with some impossible transitions removed
  GOMA::matrix<int> t_cost_;

 public:
  /**
   * @brief Construct a new OpsInput object
   *
   * @param build Whether to build the graph structures or not.
   */
  OpsInput(bool build = false);

  virtual ~OpsInput();

  inline int get_t(int i, int j) const {
    return t_cost_(i + 1, j + 1);
  }

  // getAmountOfSuccessors
  inline int get_nsucc(int k, int i) const {
    return succ_(k + 1, i + 1).size();
  }

  // getAmountOfPredecessors
  inline int get_npred(int k, int i) const {
    return pred_(k + 1, i + 1).size();
  }

// getAmountOfSuccessors
  int get_msucc(int k) const;

  // getArcId(..., true)
  inline int get_succ_inx(int k, int i, int l) const {
    return succ_inx_(k + 1, i + 1)[l];
  }

  // getArcId(..., false)
  inline int get_pred_inx(int k, int i, int l) const {
    return pred_inx_(k + 1, i + 1)[l];
  }

  // getArcId
  inline int get_inv_succ(int k, int i, int j)
    const { /* assert (inv_succ_[k](i + 1, j + 1) != -1); */
    return inv_succ_[k](i + 1, j + 1);
  }

  inline int get_A_succ_sz() const {
    return A_succ_.size();
  }

  inline int get_A_succ(int l) const {
    return A_succ_[l];
  }

  int get_pos(int k, int i, int j) const {
    return (i * getN() + j + getN() * getN() * k);
  }

  void get_pos(int pos, int &k, int &i, int &j) const;

  // getBarsStats
  void get_sync_stat(int &nsync, int &maxgsync, int &mingsync, double &avggsync)
    const;

  // getNodesStats
  double get_avg_nodes() const;
  // getNodesStats
  int get_max_nodes() const;

  // getMaxArc
  int get_max_arc() const;

  void writeStatistics(std::ostream &os) const;
  void writeStatisticsHdr(std::ostream &os) const;

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the >> operator to read an input from a json file.
   * Calls OpsInstance::operator>> and then builds the input.
   *
   * @param is Represents the inflow
   * @param ops_input The OPS input to read from the inflow
   * @return The inflow with the input read
   */
  friend std::istream &operator>>(std::istream &is, OpsInput &ops_input);

 protected:
  // -------------------------------- Test -------------------------------- //

  void test_succ();
  void test_pred();

  void test_A_succ();

  // ------------------------------- Build ------------------------------- //

  /**
   * @brief Creates the necessary structures to represent the graph.
   */
  void build();

  /** @brief Resize all the structures that represents the graph. */
  void resizeGraphStructures();

  /**
   * @brief Update the structures that represents the graph with the new arc
   * (Ji -> Jj) in the k-th sliding bar. The structures that are updated are:
   * - The successors
   * - The arcs in the k-th sliding bar
   * - The nodes that can be reached in the k-th sliding bar
   *
   * @param k The index of the sliding bar
   * @param Ji The index of the object which arc is connected towards Jj
   * @param Jj The index of the object which arc is connected from Ji
   * @param l The index of the arc (Ji -> Jj) in the k-th sliding bar.
   */
  void updateGraphStructures(
    int k, int Ji, int Jj, int &l, GOMA::matrix<int> &A_inv,
    std::vector<int> &arcs_k, std::vector<int> &nodes
  );

  /**
   * @brief Update the predecessors matrixes for the node Jj, adding the arc
   * (Ji -> Jj) in the k-th sliding bar.
   *
   * @param k The index of the sliding bar
   * @param Ji The index of the object which arc is connected towards Jj
   * @param Jj The index of the object which arc is connected from Ji
   * @param l The index of the arc (Ji -> Jj) in the k-th sliding bar.
   */
  void addPredecessor(int k, int Ji, int Jj, int &l);

  /**
   * @brief Copy the time matrix from the input instance to the t_cost_
   * matrix, truncating the first and last row to have only arcs out and
   * arcs in respectively.
   * Furthermore, removes the diagonal since an object cannot travel to
   * itself.
   */
  void processTMatrix();

  /**
   * @brief Creates the arcs matrixes for each sliding bar, creating a
   * graph with only the nodes that can be reached.
   * The arcs are created if:
   * - The time doesn't exceed the time limit
   * - No node can go to the first node.
   * - No node can be reached from the last node.
   * - No node can go to itself.
   */
  void makeGraphArcs();

  /**
   * @brief Creates the predecessors matrixes for each sliding bar,
   * creating a graph with all nodes connected between them except the
   * first one with the last one.
   */
  void makePredecessors();
};

}  // namespace emir

#endif  // _EMIR_OPS_INPUT_HPP_