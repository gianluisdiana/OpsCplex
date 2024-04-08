#ifndef _MAIN_FUNCTIONS_HPP_
#define _MAIN_FUNCTIONS_HPP_

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
void processInstance(const PathConfig &path_config, const double tolerance);

/**
 * @brief Process the model type and generates the output and log files.
 *
 * @param model_type The model type to be processed.
 * @param tolerance The tolerance for the solver.
 */
void processModelType(const std::string &model_type, const double tolerance);

/**
 * @brief Process the input file and generates the output and log files.
 *s
 * @param input_path The path to the input file to be processed.
 * @param tolerance The tolerance for the solver.
 */
void processFile(const std::string &input_path, const double tolerance);

#endif  // _MAIN_FUNCTIONS_HPP_