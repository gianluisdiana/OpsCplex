#include <ops_solver.hpp>

namespace emir {

OpsSolver::OpsSolver(const OpsInput &input, double tolerance) :
  input_(input), output_(input), tolerance_(tolerance) {
  Arc::id_counter_ = 0;
}

// ------------------------------- Operators ------------------------------- //

std::ostream &operator<<(std::ostream &os, const OpsSolver &solver) {
  return os << solver.output_;
}

}  // namespace emir