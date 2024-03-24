#include <input_parser/parser.hpp>
#include <solvers.hpp>

const input_parser::Parser createParser() {
  return input_parser::Parser()
    .addHelpOption()
    .addOption<input_parser::SingleOption>([] {
      return input_parser::SingleOption("-i", "--input")
        .addDescription("Path to the input file to be processed")
        .addDefaultValue(std::string())
        .addConstraint<const std::string &>(
          [](const auto &value) -> bool {
            return value.empty() ||
                   (!value.empty() && std::filesystem::exists(value));
          },
          "The file must exist!"
        );
    })
    .addOption<input_parser::CompoundOption>([] {
      return input_parser::CompoundOption("-m", "--models")
        .addDescription("List of models type to be processed")
        .addConstraint<const std::vector<std::string> &>(
          [](const auto &values) -> bool {
            for (const auto &value : values) {
              if (value != "A" && value != "B" && value != "C" && value != "LA" && value != "LB" && value != "LC")
                return false;
            }
            return true;
          },
          "The model types must be one of the following: A, B, C, LA, LB, LC"
        )
        .addDefaultValue(std::vector<std::string>());
    });
}

/**
 * @brief Main function to process folders filled with instances and output
 * the results.
 */
int main(int argc, char *argv[]) {
  auto parser = createParser();

  try {
    parser.parse(argc, argv);
  } catch (const input_parser::ParsingError &e) {
    std::cerr << e.what() << std::endl;
    parser.displayUsage();
    return 1;
  }

  const auto &inputPath = parser.getValue<std::string>("--input");
  const auto &models = parser.getValue<std::vector<std::string>>("--models");
  if (!inputPath.empty()) {
    processInputFile(inputPath);
  } else if (!models.empty()) {
    for (const auto &model : models) processModelType(model);
  }
  return 0;
}