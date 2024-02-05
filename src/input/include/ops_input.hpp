// clang-format off
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
 * @version 0.2.0
 * @date January 21, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of the input instance for the O.P.S
 * problem, representing the math model with a graph for each sliding bar.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <OPS_instance_t.hpp>
#include <graph.hpp>
#include <matrix.hpp>
#include <stats.hpp>

namespace emir {

/** @brief Represents a OPS instance with the data structures needed to
 * represent the math model and the graphs */
class OpsInput : public OpsInstance {
 public:
  /** @brief Construct an empty input instance. */
  OpsInput();

  // ------------------------------ Getters -------------------------------- //

  /**
   * @brief Get how many successors has the node with the given id in the k-th
   * sliding bar.
   *
   * @param sliding_bar_index index of the sliding bar
   * @param node_id The id of the object
   */
  std::size_t getAmountOfSuccessors(
    const int sliding_bar_index, const std::string &node_id
  ) const;

  /**
   * @brief Gets how many succesors has the graph that correspond to the k-th
   * sliding bar.
   *
   * @param sliding_bar_index index of the sliding bar
   */
  inline std::size_t getAmountOfSuccessors(const int sliding_bar_index) const {
    return graphs_[sliding_bar_index].getAmountOfSuccessors();
  }

  /**
   * @brief Get how many predecessors has the node with the given id in the k-th
   * sliding bar.
   *
   * @param sliding_bar_index The index of the sliding bar
   * @param node_id The id of the node in the graph
   */
  std::size_t getAmountOfPredecessors(
    const int sliding_bar_index, const std::string &node_id
  ) const;

  /**
   * @brief Gets the arc's id of the arc that connects the node with the given
   * id in the graph (either successor or predecessor) in the k-th sliding bar.
   *
   * @param sliding_bar_index The index of the sliding bar
   * @param node_id The id of the node
   * @param arc_index The index of the arc
   * @param is_successor Whether the arc is completed by a successor or not
   * @return The id of the arc
   */
  inline unsigned int getArcId(
    const int sliding_bar_index, const std::string &node_id,
    const int arc_index, const bool is_successor
  ) const {
    return graphs_[sliding_bar_index].getArcId(
      node_id, arc_index, is_successor
    );
  }

  /**
   * @brief Gets the arc's id of the arc that connects the nodes with the given
   * ids in the graph of the k-th sliding bar.
   *
   * @param sliding_bar_index The index of the sliding bar
   * @param origin_id The id of the origin node
   * @param destination_id The id of the destination node
   * @return The id of the arc
   */
  inline unsigned int getArcId(
    const int sliding_bar_index, const std::string &origin_id,
    const std::string &destination_id
  ) const {
    return graphs_[sliding_bar_index].getArcId(origin_id, destination_id);
  }

  /**
   * @brief Gets the graph that belongs to the k-th sliding bar.
   *
   * @param sliding_bar_index The index of the sliding bar
   * @return The graph that belongs to the k-th sliding bar
   */
  inline const Graph &getGraph(const int sliding_bar_index) const {
    return graphs_[sliding_bar_index];
  }

  /** @brief Gets the maximum cost of any arc in the graph */
  unsigned int getMaxArc() const;

  // ---------------------------- Stats Getters ---------------------------- //

  const std::string getStatistics() const;

  inline const std::string getStatisticsHdr() const {
    return /*OpsInput::getStatisticsHdr()*/
      "$n$\t$m$\t$\\max|J_k|$\t$\\bar{|J_k|}$\tnsync\t"
      "$\\max\\partial\\mbox{sync}$\t$\\bar{\\partial\\mbox{sync}$\t$L$\t";
  }

  // --------------------------- Utility methods --------------------------- //

  /**
   * @brief Calculates the index of the x variable in the vector
   * representation of the matrix.
   *
   * @param k The k-th sliding bar
   * @param i The i-th node
   * @param j The j-th node
   */
  inline unsigned int
  calculateXIndex(const int k, const int i, const int j) const {
    return k * getM() + i * getN() + j;
  }

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

 private:
  // Graphs with the arcs that can be reached in the k-th sliding bar
  std::vector<Graph> graphs_;

  /**
   * @brief Removes the arcs coming out of the last object (n -> i), the arcs
   * that enter the first object (i -> 1) and the arc that enters and exits at
   * the same node (i -> i).
   */
  void truncateT();

  /**
   * @brief Creates the arcs for each sliding bar, adding to each graph only the
   * nodes that can be reached. An arc is created if:
   * - The time doesn't exceed the time limit
   * - No node can go to the first node.
   * - No node can be reached from the last node.
   * - No node can go to itself.
   */
  void createGraphArcs();

  // ---------------------- Private Statistics Getters ---------------------- //

  /**
   * @brief Gets statistics about the graph. Specifically:
   * - The amount of objects that can be reached in by any sliding bar
   * - The maximum amount of bars needed to reach an object
   * - The minimum amount of bars needed to reach an object
   * - The average amount of bars needed to reach an object
   *
   * @return The statistics described above.
   */
  const Stats getBarsStats() const;

  /**
   * @brief Gets statistics about the graph. Specifically:
   * - The maximum amount of nodes reached by a sliding bar
   * - The minimum amount of nodes reached by a sliding bar
   * - The average amount of nodes reached by the sliding bars
   *
   * @return The statistics described above.
   */
  const Stats getNodesStats() const;
};

}  // namespace emir