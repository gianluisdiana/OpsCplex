// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file arc.hpp
 * @author Gian Luis Bolivar Diana
 * @version 0.2.0
 * @date February 5, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of an arc in a graph structure.
 *  An arc has a origin node, a destination node and a cost.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef _EMIR_ARC_HPP_
#define _EMIR_ARC_HPP_

#include <memory>
#include <string>

namespace emir {

/** @see Node class: src/input/include/node.hpp */
class Node;

/** @brief Represents an arc in the graph */
class Arc {
 public:
  /**
   * @brief Creates a new arc (from_node -> to_node) with an unique id
   *
   * @param from Origin node of the arc
   * @param to Destination node of the arc
   * @param cost The cost of the arc
   */
  Arc(
    const std::shared_ptr<Node> &from, const std::shared_ptr<Node> &to,
    const int cost
  );

  // ------------------------------ Getters -------------------------------- //

  /** @brief Gets the id of the arc */
  inline unsigned int getId() const {
    return id_;
  }

  /** @brief Gets the cost of the arc */
  inline int getCost() const {
    return cost_;
  }

  /** @brief Gets the id from the origin node of the arc */
  const std::string &getOriginId() const;

  /** @brief Gets the id from the destination node of the arc */
  const std::string &getDestinationId() const;

 private:
  // Keeps track of the amount of arcs created
  static unsigned int id_counter_;

  // Identifier of the arc
  const unsigned int id_;
  // Origin node of the arc
  std::shared_ptr<Node> from_;
  // Destination node of the arc
  std::shared_ptr<Node> to_;
  // The cost of the arc
  int cost_;
};

}  // namespace emir

#endif  // _EMIR_ARC_HPP_