#include <iomanip>
#include <sstream>

#include <functions.hpp>

time_t stringToDateStamp(const std::string &date_stamp) {
  struct std::tm time {};

  std::istringstream string_stream(date_stamp);
  string_stream >> std::get_time(&time, "%c");
  return mktime(&time);
}