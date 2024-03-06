#include <functions.hpp>
#include <ops_instance.hpp>

namespace emir {

const unsigned int OpsInstance::kInfiniteTime = 999'999;

OpsInstance::OpsInstance() :
  name_(), date_stamp_(), type_(), Jk_(), Kj_(), T_(), b_(), alpha_(-1), L_(0),
  scal_factor_(10) {}

OpsInstance::~OpsInstance() {}

// ----------------------------- Statistcs Data ---------------------------- //

void OpsInstance::writeStatisticsHdr(std::ostream &os) const {
  os << "ID\t";
}

void OpsInstance::writeStatistics(std::ostream &os) const {
  os << name_ << "\t";
}

// ------------------------------- Operators ------------------------------- //

std::istream &operator>>(std::istream &is, OpsInstance &ops_instance) {
  nlohmann::json json_instance;
  is >> json_instance;
  ops_instance.setFromJson(json_instance);
  // ops_instance.truncateTMatrix();
  return is;
}

std::ostream &operator<<(std::ostream &os, const OpsInstance &ops_instance) {
  return os << std::setw(2) << ops_instance.toJson();
}

// ---------------------------- Private Methods ----------------------------- //

const nlohmann::json OpsInstance::toJson() const {
  return {
    {"id", nlohmann::json::array({name_, std::ctime(&date_stamp_)})},
    {"type", type_},
    {"Jk", Jk_},
    {"b", b_},
    {"alpha", alpha_},
    {"L", L_},
    {"T", T_.toJson()}};
}

void OpsInstance::setFromJson(const nlohmann::json &json_instance) {
  if (json_instance.find("T") != json_instance.end())
    T_.setFromJson(json_instance["T"]);
  if (json_instance.find("id") != json_instance.end() && json_instance["id"].size() >= 2) {
    name_ = json_instance["id"][0].get<std::string>();
    date_stamp_ = stringToDateStamp(json_instance["id"][1].get<std::string>());
  }
  if (json_instance.find("type") != json_instance.end())
    type_ = json_instance["type"].get<int>();
  if (json_instance.find("b") != json_instance.end())
    b_ = json_instance["b"].get<std::vector<int>>();
  if (json_instance.find("Jk") != json_instance.end()) {
    Jk_ = json_instance["Jk"].get<std::vector<std::vector<int>>>();
    resetKjMatrix();
  }
  if (json_instance.find("alpha") != json_instance.end())
    alpha_ = json_instance["alpha"].get<double>();
  if (json_instance.find("L") != json_instance.end())
    L_ = json_instance["L"].get<int>();
}

void OpsInstance::truncateTMatrix() {
  const auto m = T_.getRowsAmount();
  const auto n = T_.getColsAmount();
  for (auto i = 0; i < m; ++i) {
    for (auto j = 0; j < n; ++j) {
      if (T_(i, j) > L_) T_(i, j) = OpsInstance::kInfiniteTime + 1;
    }
  }
}

void OpsInstance::resetKjMatrix() {
  Kj_.clear();
  Kj_.resize(getN());
  for (auto k = 0; k < getM(); ++k) {
    for (const auto i : getJk(k)) Kj_[i].push_back(k);
  }
}

}  // namespace emir
