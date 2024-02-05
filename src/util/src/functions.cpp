#include <iomanip>
#include <sstream>

#include <functions.hpp>

const time_t stringToDateStamp(const std::string &date_stamp) {
  std::istringstream ss(date_stamp);
  struct std::tm tm;
  ss >> std::get_time(&tm, "%c");
  return mktime(&tm);
}