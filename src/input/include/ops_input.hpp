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

#ifndef EMIR_OPS_INPUT_HPP_
#define EMIR_OPS_INPUT_HPP_

#include <graph.hpp>
#include <ops_instance.hpp>

namespace emir {

/**
 * @brief Represents a OPS instance with the data structures needed to represent
 * the math model and the graphs.
 */
class OpsInput : public OpsInstance {
 public:
  /** @brief Construct an empty input instance. */
  OpsInput() = default;

  /** @brief Copies the input instance */
  OpsInput(const OpsInput &) = default;

  /** @brief Moves the input instance */
  OpsInput(OpsInput &&) = default;

  /** @brief Resets the arc id counter. */
  ~OpsInput();

  // ------------------------------ Getters -------------------------------- //

  /**
   * @brief Gets the graph that belongs to the k-th sliding bar.
   *
   * @param graph_idx The index of the sliding bar
   * @return The graph that belongs to the k-th sliding bar
   */
  [[nodiscard]] const Graph &getGraph(const int graph_idx) const {
    return graphs_[graph_idx];
  }

  /** @brief Gets the maximum cost of any arc in the graph */
  [[nodiscard]] unsigned int getMaxArc() const;

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the = operator to copy the input instance.
   *
   * @param[in] unused The input instance to copy
   * @return The copied input instance
   */
  OpsInput &operator=(const OpsInput &) = default;

  /**
   * @brief Overload of the = operator to move the input instance.
   *
   * @param[in] Unused The input instance to move
   * @return The moved input instance
   */
  OpsInput &operator=(OpsInput &&) = default;

  /**
   * @brief Overload of the >> operator to read an input from a json file.
   * Calls OpsInstance::operator>> and then builds the input.
   *
   * @param input_stream Represents the inflow
   * @param ops_input The OPS input to read from the inflow
   * @return The inflow with the input read
   */
  friend std::istream &
  operator>>(std::istream &input_stream, OpsInput &ops_input);

 private:
  // ----------------------------- Attributes ------------------------------ //

  // Graphs with the arcs that can be reached in the k-th sliding bar
  std::vector<Graph> graphs_;

  // ------------------------------- Methods ------------------------------- //

  /** @brief Create all the graphs and add the arcs to each one. */
  void createGraphs();

  /**
   * @brief Creates the arcs for each sliding bar, adding to each graph only the
   * nodes that can be reached. The arcs created are the following:
   * - The first node goes to all the nodes in the sliding bar.
   * - All the nodes in the sliding bar go to the last node.
   * - Each node goes to all the other nodes in the sliding bar.
   */
  void addGraphArcs(int graph_idx);
};

}  // namespace emir

#endif  // EMIR_OPS_INPUT_HPP_