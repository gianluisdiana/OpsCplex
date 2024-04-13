// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file node.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.2.0
 * @date February 5, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of a basic node (without values) in a
 *  graph. A node is a point in the graph that can be connected to other nodes
 *  through arcs.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef _EMIR_NODE_HPP_
#define _EMIR_NODE_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <arc.fwd.hpp>

namespace emir {

/** @brief Represents a node in the graph */
class Node {
 public:
  /**
   * @brief Creates a new node with the given id
   *
   * @param node_id Id of the node
   */
  explicit Node(unsigned int node_id = 0);

  // ------------------------------ Getters -------------------------------- //

  /** @brief Gets the name of the node */
  [[nodiscard]] unsigned int getId() const {
    return id_;
  }

  /**
   * @brief Gets the id of the arcs that connect the node with its successors
   */
  [[nodiscard]] std::vector<unsigned int> getSuccessorsArcsId() const {
    return Node::getArcsId(successors_);
  }

  /**
   * @brief Gets the id of the arcs that connect the node with its predecessors
   */
  [[nodiscard]] std::vector<unsigned int> getPredecessorsArcsId() const {
    return Node::getArcsId(predecessors_);
  }

  // ------------------------------- Adders -------------------------------- //

  /**
   * @brief Adds a new successor to the node
   *
   * @param successor Successor to add
   * @param arc The arc that connects them
   */
  void addSuccessor(
    const std::shared_ptr<Node> &successor, const std::shared_ptr<Arc> &arc
  ) {
    successors_.emplace_back(successor, arc);
  }

  /**
   * @brief Adds a new predecessor to the node
   *
   * @param predecessor Predecessor to add
   * @param arc The arc that connects them
   */
  void addPredecessor(
    const std::shared_ptr<Node> &predecessor, const std::shared_ptr<Arc> &arc
  ) {
    predecessors_.emplace_back(predecessor, arc);
  }

 private:
  // -------------------------- Type Definitions --------------------------- //

  // Pairs of node and arc
  using NodeArcPairs =
    std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<Arc>>>;

  // ----------------------------- Attributes ------------------------------ //

  // Identifier of the node
  unsigned int id_;
  // Successors of the node
  NodeArcPairs successors_;
  // Predecessors of the node
  NodeArcPairs predecessors_;

  // ------------------------------- Getters ------------------------------- //

  /**
   * @brief Gets the id of the arcs that connect the node with its successors
   * or predecessors.
   *
   * @param nodes_with_arcs Map of nodes with their respective arcs
   * @return The id of the arcs that connect the node with its successors or
   * predecessors
   */
  [[nodiscard]] static std::vector<unsigned int>
  getArcsId(const NodeArcPairs &nodes_with_arcs);
};

}  // namespace emir

#endif  // _EMIR_NODE_HPP_