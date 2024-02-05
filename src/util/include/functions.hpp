#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <ctime>
#include <string>

/**
 * @brief Converts a string to a date stamp.
 *
 * @param date_stamp String to be converted.
 * @return The date stamp of the string.
 */
const time_t stringToDateStamp(const std::string &date_stamp);

#endif  // _FUNCTIONS_HPP_