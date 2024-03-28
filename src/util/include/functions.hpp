#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <ctime>
#include <fstream>

#include <local_type_traits.hpp>

/**
 * @brief Converts a string to a date stamp.
 *
 * @param date_stamp String to be converted.
 * @return The date stamp of the string.
 */
time_t stringToDateStamp(const std::string &date_stamp);

/**
 * @brief Import the data from a file and convert it to a given type.
 *
 * @tparam T Type to be converted to.
 * @param file_name Name of the file to be imported.
 * @return The data from the file converted to the given type.
 */
template <typename T, typename = std::enable_if_t<is_readable<T &>::value>>
T createFromFile(const std::string &file_name) {
  std::ifstream file(file_name);
  T data;
  file >> data;
  file.close();
  return data;
}

#endif  // _FUNCTIONS_HPP_