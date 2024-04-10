// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file main.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the main function to process instances and output the
 * results.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <iostream>
#include <string>
#include <vector>

#include <input_parser/parser.hpp>
#include <input_parser/parsing_error.hpp>

#include <main_functions.hpp>
#include <ops_error.hpp>

/**
 * @brief Main function to process folders filled with instances and output
 * the results.
 */
int secureMain(int argc, char *argv[]) {
  auto parser = createParser();

  try {
    parser.parse(argc, argv);
  } catch (const input_parser::ParsingError &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  const auto &input_path = parser.getValue<std::string>("--input");
  const auto &models = parser.getValue<std::vector<std::string>>("--models");
  const auto tolerance = parser.getValue<double>("--tolerance");
  if (!input_path.empty()) {
    processFile(input_path, tolerance);
  } else if (!models.empty()) {
    for (const auto &model : models) { processModelType(model, tolerance); }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  try {
    return secureMain(argc, argv);
  } catch (const emir::OpsError &e) {
    std::cerr << "OspError: " << e.what() << '\n';
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}