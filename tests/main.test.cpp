#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <functions.hpp>
#include <ops_cplex_solver.hpp>

namespace fs = std::filesystem;

/**
 * @brief Test the given model class.
 * Ensures that the profit obtained by the solver is the same as the one
 * obtained by the solution file and that the solver does not throw any
 * exceptions during the process.
 *
 * @param model_class The model class to be tested.
 */
void testModelClass(const std::string &model_class) {
  const auto input_folder = std::format("data/{}/instances", model_class);
  const auto solution_folder = std::format("data/{}/outputs/", model_class);
  const double tolerance = 1e-4;
  nlohmann::json solution;
  std::stringstream string_stream;
  for (const auto &entry : fs::directory_iterator(input_folder)) {
    std::ifstream solution_file(
      solution_folder + entry.path().filename().string()
    );
    emir::OpsCplexSolver solver(
      createFromFile<emir::OpsInput>(entry.path()), tolerance
    );
    solver.addLog(string_stream);
    ASSERT_NO_THROW(solver.solve());
    solution_file >> solution;
    EXPECT_EQ(solver.getProfit(), solution["profit"].get<double>());
    string_stream.str(std::string());
  }
}

TEST(OpsTest_FamilyH, TotalProfitOneBandNeeded) {
  testModelClass("A");
}

TEST(OpsTest_FamilyH, TotalProfitTwoBandsNeeded) {
  testModelClass("B");
}

TEST(OpsTest_FamilyH, TotalProfitThreeBandsNeeded) {
  testModelClass("C");
}

TEST(OpsTest_FamilyE, TotalProfitOneBandNeeded) {
  testModelClass("LA");
}

TEST(OpsTest_FamilyE, TotalProfitTwoBandsNeeded) {
  testModelClass("LB");
}

TEST(OpsTest_FamilyE, TotalProfitThreeBandsNeeded) {
  testModelClass("LC");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}