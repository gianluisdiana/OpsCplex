// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_input.hpp
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

#include <graph.hpp>
#include <ops_instance.hpp>
#include <stats.hpp>

namespace emir {

/** @brief Represents a OPS instance with the data structures needed to
 * represent the math model and the graphs */
class OpsInput : public OpsInstance {
 public:
  /** @brief Construct an empty input instance. */
  OpsInput();

  /** @brief Resets the arc id counter. */
  ~OpsInput();

  // ------------------------------ Getters -------------------------------- //

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

  // TODO: ERASE
  const std::string getStatistics() const;

  inline const std::string getStatisticsHdr() const {
    return /*OpsInput::getStatisticsHdr()*/
      "$n$\t$m$\t$\\max|J_k|$\t$\\bar{|J_k|}$\tnsync\t"
      "$\\max\\partial\\mbox{sync}$\t$\\bar{\\partial\\mbox{sync}$\t$L$\t";
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
  // ----------------------------------------------------------------------- //
  // ----------------------------- Attributes ------------------------------ //
  // ----------------------------------------------------------------------- //

  // Graphs with the arcs that can be reached in the k-th sliding bar
  std::vector<Graph> graphs_;

  // ----------------------------------------------------------------------- //
  // ------------------------------- Methods ------------------------------- //
  // ----------------------------------------------------------------------- //

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