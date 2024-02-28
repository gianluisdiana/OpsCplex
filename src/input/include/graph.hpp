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
  Graph();

  // ------------------------------ Getters -------------------------------- //

  /**
   * @brief Gets the amount of successors of a node
   *
   * @param id Id of the node
   * @return The amount of successors of the node
   */
  inline std::size_t getAmountOfSuccessors(const std::string &id) const {
    return nodes_.at(id)->getAmountOfSuccessors();
  }

  /** @brief Gets the amount of successors of the graph */
  std::size_t getAmountOfSuccessors() const;

  /**
   * @brief Gets the amount of predecessors of a node
   *
   * @param id Id of the node
   * @return The amount of predecessors of the node
   */
  inline std::size_t getAmountOfPredecessors(const std::string &id) const {
    return nodes_.at(id)->getAmountOfPredecessors();
  }

  /**
   * @brief Gets the arc's id of the arc that connects the given node
   * (either successor or predecessor).
   *
   * @param id Id of the node
   * @param arc_index The index of the arc
   * @param is_successor Whether the arc is completed by a successor or not
   * @return The id of the arc
   */
  unsigned int getArcId(
    const std::string &id, const int arc_index, const bool &is_successor
  ) const;

  /**
   * @brief Gets the arc's id of the arc that connects the given node
   * (either successor or predecessor).
   *
   * @param origin_id Id of the origin node
   * @param destination_id Id of the destination node
   * @return The id of the arc
   */
  inline unsigned int getArcId(
    const std::string &origin_id, const std::string &destination_id
  ) const {
    return nodes_.at(origin_id)->getArcTo(nodes_.at(destination_id))->getId();
  }

  /** @brief Gets the ids of the nodes of the graph */
  const std::vector<std::string> getNodesId() const;

  /** @brief Gets the amount of nodes of the graph */
  inline const std::size_t getAmountOfNodes() const {
    return nodes_.size();
  }

  /** @brief Gets the arcs of the graph */
  inline const std::vector<Arc> &getArcs() const {
    return arcs_;
  }

  // ------------------------------- Adders ------------------------------- //

  /**
   * @brief Adds a new arc to the graph
   *
   * @param from_id Id of the origin node
   * @param to_id Id of the destination nodeç
   * @param cost Cost of the arc
   */
  inline void addArc(const int from_id, const int to_id, const int cost) {
    addArc(std::to_string(from_id), std::to_string(to_id), cost);
  }

  /**
   * @brief Adds a new arc to the graph
   *
   * @param from_id Id of the origin node
   * @param to_id Id of the destination node
   * @param cost Cost of the arc
   */
  void
  addArc(const std::string &from_id, const std::string &to_id, const int cost);

  /**
   * @brief Checks if the graph has a node with the given id
   */
  inline bool hasNode(const std::string &id) const {
    return nodes_.find(id) != nodes_.end();
  }

 private:
  // The arcs of the graph
  std::vector<Arc> arcs_;
  // Nodes of the graph
  std::map<std::string, std::shared_ptr<Node>> nodes_;

  /**
   * @brief Gets the node from the map, if it doesn't exist it creates it and
   * returns it.
   *
   * @param id Id of the node to search
   * @return The node with the given id.
   */
  std::shared_ptr<Node> searchNode(const std::string &id);
};

}  // namespace emir

#endif  // _EMIR_GRAPH_HPP_