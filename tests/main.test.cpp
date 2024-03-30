#include <filesystem>

#include <gtest/gtest.h>

#include <functions.hpp>
#include <ops_cplex_solver.hpp>

namespace fs = std::filesystem;

void testFolder(const std::string &path) {
  const auto &input_folder = "data/input/" + path + "/instances";
  const auto &solution_folder = "data/output/" + path + "/";
  nlohmann::json solution;
  std::stringstream string_stream;
  for (const auto &entry : fs::directory_iterator(input_folder)) {
    std::ifstream solution_file(
      solution_folder + entry.path().filename().string()
    );
    const auto solver =
      solve<emir::OpsCplexSolver>(entry.path().string(), 1e-4, string_stream);
    solution_file >> solution;
    EXPECT_EQ(solver.getProfit(), solution["profit"].get<int>());
  }
}

TEST(OpsTest, TotalProfitAssertionsOneBandNeeded) {
  testFolder("A");
}

TEST(OpsTest, TotalProfitAssertionsTwoBandsNeeded) {
  testFolder("B");
}

// TEST(OpsTest, TotalProfitAssertionsThreeBandsNeeded) {
//   testFolder("C");
// }

// TEST(OpsTest, TotalProfitAssertionsOneBandNeeded) {
//   testFolder("LA");
// }

// TEST(OpsTest, TotalProfitAssertionsTwoBandsNeeded) {
//   testFolder("LB");
// }

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}