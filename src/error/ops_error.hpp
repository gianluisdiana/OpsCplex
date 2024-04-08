#ifndef _EMIR_OPS_ERROR_HPP_
#define _EMIR_OPS_ERROR_HPP_

#include <exception>
#include <format>
#include <string>

namespace emir {

/** @brief Represents an error generated inside the O.P.S. architecture */
class OpsError : public std::exception {
 public:
  /**
   * @brief Constructs an OpsError with the given message, formatted with the
   * given arguments.
   *
   * @tparam Args Types of the arguments to be formatted.
   * @param format Format string to be used.
   * @param args Arguments to be formatted.
   */
  template <typename... Args>
  explicit OpsError(std::format_string<Args...> format, Args &&...args) :
    message_ {std::format(format, args...)} {}

  /** @brief Returns the message to be shown. */
  inline const char *what() const noexcept override {
    return message_.c_str();
  }

 private:
  // The message to be shown.
  const std::string message_;
};

}  // namespace emir

#endif  // _EMIR_OPS_ERROR_HPP_