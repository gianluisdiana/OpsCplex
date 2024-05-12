// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file main_functions.cpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing implementations of the functions only used in the main file
 * (or in this file).
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <input_parser/parser.hpp>

#include <functions.hpp>
#include <main_functions.hpp>
#include <ops_cplex_solver.hpp>

namespace fs = std::filesystem;

input_parser::Parser createParser() {
  return input_parser::Parser()
    .addHelpOption()
    .addOption([] {
      return input_parser::SingleOption("-i", "--input")
        .addDescription("Path to the input file to be processed")
        .addDefaultValue(std::string())
        .addConstraint<std::string>(
          [](const auto &value) -> bool {
            return value.empty() || std::filesystem::exists(value);
          },
          "The file must exist!"
        );
    })
    .addOption([] {
      return input_parser::CompoundOption("-c", "--classes")
        .addDescription("List of class to be processed")
        .addDefaultValue(std::vector<std::string>())
        .addConstraint<std::vector<std::string>>(
          [](const auto &values) -> bool {
            return std::ranges::all_of(values, [](const auto &value) {
              return value == "A" || value == "B" || value == "C" ||
                     value == "LA" || value == "LB" || value == "LC";
            });
          },
          "The classes must be one of the following: A, B, C, LA, LB, LC"
        )
        .addConstraint<std::vector<std::string>>(
          [](const auto &values) -> bool {
            return std::ranges::all_of(
              values,
              [values](const auto &value) -> bool {
                return std::ranges::count(values, value) == 1;
              }
            );
          },
          "The classes cannot be specified more than once"
        );
    })
    .addOption([] {
      return input_parser::SingleOption("-t", "--tolerance")
        .addDescription("Tolerance for the solver")
        .addDefaultValue(std::string("1e-4"))
        .toDouble()
        .transformBeforeCheck()
        .addConstraint<double>(
          [](const auto &value) -> bool { return value > 0; },
          "The tolerance must be greater than 0"
        );
    });
}

void processInstance(const PathConfig &path_config, const double tolerance) {
  std::ofstream output_os(path_config.output_path);
  std::stringstream string_stream;
  output_os << solve<emir::OpsCplexSolver>(
    path_config.input_path, tolerance, string_stream
  );
}

void processModelClass(const std::string &model_class, const double tolerance) {
  const auto &input_folder = "data/" + model_class + "/instances/";
  const auto &output_folder = "data/" + model_class + "/outputs/";
  if (!fs::exists(output_folder)) { fs::create_directory(output_folder); }
  for (const auto &file : fs::directory_iterator(input_folder)) {
    std::cout << file.path() << '\n';
    processInstance(
      {.input_path = file.path(),
       .output_path = output_folder + file.path().filename().string()},
      tolerance
    );
  }
}