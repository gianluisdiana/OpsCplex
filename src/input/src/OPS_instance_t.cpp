#include <OPS_instance_t.hpp>
#include <functions.hpp>

namespace emir {

const unsigned int OpsInstance::kInfiniteTime = 999'999;

OpsInstance::OpsInstance(void) :
  name_(), date_stamp_(), type_(), Jk_(), Kj_(), T_(), b_(), alpha_(-1), L_(0),
  scal_factor_(10) {}

OpsInstance::~OpsInstance(void) {}

// ------------------------------- Getters --------------------------------- //

int OpsInstance::get_max_Jk(void) const {
  int max = 0;

  for (auto Jk : Jk_)
    if ((int)(Jk.size()) > max) max = Jk.size();

  return max;
}

// ----------------------------- Statistcs Data ---------------------------- //

void OpsInstance::write_statistics_hdr(std::ostream &os) const {
  os << "ID\t";
}

void OpsInstance::write_statistics(std::ostream &os) const {
  os << name_ << "\t";
}

// ------------------------------- Operators ------------------------------- //

std::istream &operator>>(std::istream &is, OpsInstance &ops_instance) {
  json json_instance;
  is >> json_instance;
  ops_instance.setFromJson(json_instance);
  ops_instance.truncateTMatrix();
  return is;
}

std::ostream &operator<<(std::ostream &os, const OpsInstance &ops_instance) {
  return os << std::setw(2) << ops_instance.toJson();
}

// ---------------------------- Private Methods ----------------------------- //

json OpsInstance::toJson() const {
  json json_file = {
    {"id", json::array({name_, get_instance_stamp()})},
    {"type", type_},
    {"Jk", Jk_},
    {"b", b_},
    {"alpha", alpha_},
    {"L", L_}};
  T_.get_json(json_file["T"]);
  return json_file;
}

void OpsInstance::setFromJson(const json &json_instance) {
  if (json_instance.find("T") != json_instance.end())
    T_.set_json(json_instance["T"]);
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
    make_Kj();
  }
  if (json_instance.find("alpha") != json_instance.end())
    alpha_ = json_instance["alpha"].get<double>();
  if (json_instance.find("L") != json_instance.end())
    L_ = json_instance["L"].get<int>();
}

void OpsInstance::truncateTMatrix() {
  const int m = T_.get_m();
  const int n = T_.get_n();
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (T_(i, j) >= L_) T_(i, j) = OpsInstance::kInfiniteTime + 1;
    }
  }
}

void OpsInstance::make_Kj(void) {
  Kj_.clear();

  const int K = get_m();

  Kj_.resize(get_n());

  for (int k = 0; k < K; k++) {
    const std::vector<int> &Jk = get_Jk(k);

    for (int i : Jk) Kj_[i].push_back(k);
  }
}

}  // namespace emir
