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

#ifndef EMIR_ARC_HPP_
#define EMIR_ARC_HPP_

#include <memory>

#include <node.fwd.hpp>

namespace emir {

/** @brief Represents an arc in the graph */
class Arc {
 public:
  /** @brief Resets the arc id counter */
  static void resetIdCounter();

  /**
   * @brief Creates a new arc (from_node -> to_node) with an unique id
   *
   * @param from Origin node of the arc
   * @param cost The cost of the arc
   * @param to Destination node of the arc
   */
  Arc(
    std::shared_ptr<Node> origin, int cost, std::shared_ptr<Node> destination
  );

  // ------------------------------ Getters -------------------------------- //

  /** @brief Gets the id of the arc */
  [[nodiscard]] unsigned int getId() const {
    return id_;
  }

  /** @brief Gets the cost of the arc */
  [[nodiscard]] int getCost() const {
    return cost_;
  }

  /** @brief Gets the id from the origin node of the arc */
  [[nodiscard]] unsigned int getOriginId() const;

  /** @brief Gets the id from the destination node of the arc */
  [[nodiscard]] unsigned int getDestinationId() const;

 private:
  // Keeps track of the amount of arcs created
  static unsigned int id_counter_;

  // ----------------------------- Attributes ------------------------------ //

  // Identifier of the arc
  unsigned int id_;
  // Origin node of the arc
  std::shared_ptr<Node> origin_;
  // The cost of the arc
  int cost_;
  // Destination node of the arc
  std::shared_ptr<Node> destination_;
};

}  // namespace emir

#endif  // EMIR_ARC_HPP_