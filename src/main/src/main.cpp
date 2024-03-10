#include <solvers.hpp>

/**
 * @brief Main function to process folders filled with instances and output
 * the results.
 */
int main(int argc, char *argv[]) {
  processModelType("A");
  processModelType("B");
  // processModelType("C");
  processModelType("LA");
  processModelType("LB");
  // processModelType("LC");
  return 0;
}