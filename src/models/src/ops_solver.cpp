#include <iostream>

#include <ops_input.hpp>
#include <ops_solver.hpp>

namespace emir {

OpsSolver::OpsSolver(const OpsInput &input, const double tolerance) :
  input_ {input}, output_ {input}, tolerance_ {tolerance} {}

OpsSolver::OpsSolver(OpsInput &&input, const double tolerance) :
  input_ {std::move(input)}, output_ {input_}, tolerance_ {tolerance} {}

// ------------------------------- Operators ------------------------------- //

std::ostream &operator<<(std::ostream &output_stream, const OpsSolver &solver) {
  return output_stream << solver.output_;
}

}  // namespace emir