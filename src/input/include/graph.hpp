// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file graph.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.2.0
 * @date February 5, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of a bidirectional graph.
 *  A graph is a set of nodes and arcs that connect them.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef _EMIR_GRAPH_HPP_
#define _EMIR_GRAPH_HPP_

#include <arc.hpp>
#include <node.hpp>

namespace emir {

/** @brief Represents a digraph */
class Graph {
 public:
  /** @brief Creates a new empty graph */
  Graph() = default;

  // ------------------------------ Getters -------------------------------- //

  /** @brief Gets the ids of the nodes of the graph */
  std::vector<unsigned int> getNodesId() const;

  /** @brief Gets the arcs of the graph */
  inline const std::vector<Arc> &getArcs() const {
    return arcs_;
  }

  /**
   * @brief Gets the id of each arc that connects the given node to its
   * successors.
   *
   * @param id Id of the node to search
   * @return The id of each arc that connects the given node to its successors.
   */
  inline std::vector<unsigned int>
  getSuccessorsArcsId(const unsigned int id) const {
    return nodes_.at(id)->getSuccessorsArcsId();
  }

  /**
   * @brief Gets the id of each arc that connects the given node to its
   * predecessors.
   *
   * @param id Id of the node to search
   * @return The id of each arc that connects the given node to its
   * predecessors.
   */
  inline std::vector<unsigned int>
  getPredecessorsArcsId(const unsigned int id) const {
    return nodes_.at(id)->getPredecessorsArcsId();
  }

  // ------------------------------- Adders ------------------------------- //

  /**
   * @brief Adds a new arc to the graph
   *
   * @param from_id Id of the origin node
   * @param to_id Id of the destination node
   * @param cost Cost of the arc
   */
  void
  addArc(const unsigned int from_id, const unsigned int to_id, const int cost);

 private:
  // The arcs of the graph
  std::vector<Arc> arcs_;
  // Nodes of the graph
  std::map<unsigned int, std::shared_ptr<Node>> nodes_;

  /**
   * @brief Gets the node from the map, if it doesn't exist it creates it and
   * returns it.
   *
   * @param node_id Id of the node to search
   * @return The node with the given id.
   */
  const std::shared_ptr<Node> &searchNode(const unsigned int node_id);
};

}  // namespace emir

#endif  // _EMIR_GRAPH_HPP_