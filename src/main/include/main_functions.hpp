// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file main_functions.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing declarations of functions only used in the main file
 * (or in this file).
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex * @endlink
 * @see Selective routing problem with synchronization @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef MAIN_FUNCTIONS_HPP_
#define MAIN_FUNCTIONS_HPP_

#include <string>

#include <input_parser/parser.hpp>

/**
 * @brief Creates a parser for the input arguments.
 * The parser is created with the following options:
 * `--input`: The path to the input file to be processed.
 * `--models`: The model types to be processed.
 * `--tolerance`: The tolerance for the solver.
 * `--help`: Shows the usage of the program.
 *
 * @return The parser created.
 */
input_parser::Parser createParser();

/** @brief Configuration with the input and output paths. */
struct PathConfig;

/**
 * @brief Creates an output file with the solution of the given input file.
 *
 * @param path_config The configuration with the input and output paths.
 * @param tolerance The tolerance for the solver.
 */
void processInstance(const PathConfig &path_config, double tolerance);

/**
 * @brief Process the model type and generates the output and log files.
 *
 * @param model_type The model type to be processed.
 * @param tolerance The tolerance for the solver.
 */
void processModelType(const std::string &model_type, double tolerance);

/**
 * @brief Process the input file and generates the output and log files.
 *s
 * @param input_path The path to the input file to be processed.
 * @param tolerance The tolerance for the solver.
 */
void processFile(const std::string &input_path, double tolerance);

#endif  // MAIN_FUNCTIONS_HPP_