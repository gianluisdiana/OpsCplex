#include <iostream>

#include <main_functions.hpp>

/**
 * @brief Main function to process folders filled with instances and output
 * the results.
 */
int secureMain(int argc, char *argv[]) {
  input_parser::Parser parser = createParser();

  try {
    parser.parse(argc, argv);
  } catch (const input_parser::ParsingError &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  const std::string &input_path =
    parser.getValue<decltype(input_path)>("--input");
  const std::vector<std::string> &models =
    parser.getValue<decltype(models)>("--models");
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