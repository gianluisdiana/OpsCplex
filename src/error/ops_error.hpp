// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file ops_error.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of a basic error generated inside
 * the O.P.S. architecture. It takes a string and formats it with the given
 * arguments to generate the error message.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Selective routing problem with synchronization: @link https://www.sciencedirect.com/science/article/pii/S0305054821002161?ref=cra_js_challenge&fr=RR-1 @endlink
 * @see EMIR Telescope: @link https://www.gtc.iac.es/instruments/emir/ @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

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
    message_ {std::format(format, std::forward<Args>(args)...)} {}

  /** @brief Returns the message to be shown. */
  [[nodiscard]] const char *what() const noexcept override {
    return message_.c_str();
  }

 private:
  // ----------------------------- Attributes ------------------------------ //

  // The message to be shown.
  std::string message_;
};

}  // namespace emir

#endif  // _EMIR_OPS_ERROR_HPP_