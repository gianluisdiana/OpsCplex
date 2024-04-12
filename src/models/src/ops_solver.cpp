// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_solver.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the implementation of the general solver for the O.P.S.
 * problem.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <iostream>
#include <utility>

#include <ops_input.hpp>
#include <ops_solver.hpp>

namespace emir {

OpsSolver::OpsSolver(const OpsInput &input, const double tolerance) :
  input_ {input}, output_ {input}, tolerance_ {tolerance} {}

OpsSolver::OpsSolver(OpsInput &&input, const double tolerance) :
  input_ {std::move(input)}, output_ {input_}, tolerance_ {tolerance} {}

// ------------------------------- Operators ------------------------------- //

std::ostream &operator<<(std::ostream &output_stream, const OpsSolver &solver) {
  return output_stream << solver.output_;
}

}  // namespace emir