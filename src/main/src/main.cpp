#include <iostream>

#include <main_functions.hpp>

/**
 * @brief Main function to process folders filled with instances and output
 * the results.
 */
int secureMain(int argc, char *argv[]) {
  auto parser = createParser();

  try {
    parser.parse(argc, argv);
  } catch (const input_parser::ParsingError &e) {
    std::cerr << e.what() << '\n' << parser.usage() << '\n';
    return 1;
  }

  const auto &input_path = parser.getValue<std::string>("--input");
  const auto &models = parser.getValue<std::vector<std::string>>("--models");
  if (!input_path.empty()) {
    processFile(input_path);
  } else if (!models.empty()) {
    for (const auto &model : models) { processModelType(model); }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  try {
    return secureMain(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}