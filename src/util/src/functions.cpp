#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <functions.hpp>

std::time_t stringToDateStamp(const std::string &date_stamp) {
  struct std::tm time {};

  std::istringstream string_stream(date_stamp);
  string_stream >> std::get_time(&time, "%c");
  return std::mktime(&time);
}