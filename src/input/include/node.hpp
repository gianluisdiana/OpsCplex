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
   * @param id Id of the node
   */
  Node(const std::string &id = "") : id_(id) {}

  // ------------------------------ Getters -------------------------------- //

  /** @brief Gets the name of the node */
  inline const std::string &getId() const {
    return id_;
  }

  /** @brief Gets the amount of successors of the node */
  inline std::size_t getAmountOfSuccessors() const {
    return successors_.size();
  }

  /** @brief Gets the amount of predecessors of the node */
  inline std::size_t getAmountOfPredecessors() const {
    return predecessors_.size();
  }

  /**
   * @brief Gets the arc that conects to the i-th successor of the node
   *
   * @param i Index of the successor
   * @return The arc that conects to the i-th successor of the node
   */
  std::shared_ptr<Arc> getSuccessorArc(const int &i) const;

  /**
   * @brief Gets the arc that conects to the i-th predecessor of the node
   *
   * @param i Index of the predecessor
   * @return The arc that conects to the i-th predecessor of the node
   */
  std::shared_ptr<Arc> getPredecessorArc(const int &i) const;

  /**
   * @brief Gets the arc that connects the node with the given node
   *
   * @param node Node that the arc connects to
   * @return The arc that connects the node with the given node
   */
  inline std::shared_ptr<Arc> getArcTo(std::shared_ptr<Node> node) const {
    return successors_.at(node);
  }

  /**
   * @brief Gets the id of the arcs that connect the node with its successors
   */
  inline const std::vector<unsigned int> getSuccessorsArcsId() const {
    return getArcsId(successors_);
  }

  /**
   * @brief Gets the id of the arcs that connect the node with its predecessors
   */
  inline const std::vector<unsigned int> getPredecessorsArcsId() const {
    return getArcsId(predecessors_);
  }

  /**
   * @brief Adds a new successor to the node
   *
   * @param successor Successor to add
   * @param arc The arc that connects them
   */
  inline void
  addSuccessor(std::shared_ptr<Node> successor, std::shared_ptr<Arc> arc) {
    successors_[successor] = arc;
  }

  /**
   * @brief Adds a new predecessor to the node
   *
   * @param predecessor Predecessor to add
   * @param arc The arc that connects them
   */
  inline void
  addPredecessor(std::shared_ptr<Node> predecessor, std::shared_ptr<Arc> arc) {
    predecessors_[predecessor] = arc;
  }

 private:
  // Identifier of the node
  const std::string id_;
  // Successors of the node
  std::map<std::shared_ptr<Node>, std::shared_ptr<Arc>> successors_;
  // Predecessors of the node
  std::map<std::shared_ptr<Node>, std::shared_ptr<Arc>> predecessors_;

  /**
   * @brief Gets the id of the arcs that connect the node with its successors
   * or predecessors.
   *
   * @param nodesWithArcs Map of nodes with their respective arcs
   * @return The id of the arcs that connect the node with its successors or
   * predecessors
   */
  const std::vector<unsigned int> getArcsId(
    const std::map<std::shared_ptr<Node>, std::shared_ptr<Arc>> &nodesWithArcs
  ) const;
};

}  // namespace emir

#endif  // _EMIR_NODE_HPP_