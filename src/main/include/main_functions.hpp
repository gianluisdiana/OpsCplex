#ifndef _MAIN_FUNCTIONS_HPP_
#define _MAIN_FUNCTIONS_HPP_

#include <string>

#include <input_parser/parser.hpp>

input_parser::Parser createParser();

/**
 * @brief Creates an output file with the solution of the given input file.
 *
 * @param input_path The path to the input file to be processed.
 * @param output_path The path to the output file to be generated.
 */
void processInstance(
  const std::string &input_path, const std::string &output_path
);

/**
 * @brief Process the model type and generates the output and log files.
 *
 * @param model_type The model type to be processed.
 */
void processModelType(const std::string &model_type);

/**
 * @brief Process the input file and generates the output and log files.
 *
 * @param input_path The path to the input file to be processed.
 */
void processFile(const std::string &input_path);

#endif  // _MAIN_FUNCTIONS_HPP_