#include <filesystem>

#include <gtest/gtest.h>

#include <functions.hpp>
#include <ops_bc.hpp>

namespace fs = std::filesystem;

// Demonstrate some basic assertions.
TEST(OpsTest, TotalProfitAssertions) {
  const auto path = "data/input/";
  std::ofstream log_os("data/test_log.txt");
  for (const auto &entry : fs::directory_iterator(path)) {
    const auto instance = createFromFile<emir::OpsInput>(entry.path());
    emir::OpsCplexSolver solver(instance, 1e-4);
    solver.addLog(log_os);
    solver.solve();
    EXPECT_NE(solver.getProfit(), 0);
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}