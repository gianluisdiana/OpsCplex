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
  } catch (...) {
    std::cerr << "Unknown error occurred.\n";
    return 1;
  }
}