#ifndef _LOCAL_TYPE_TRAITS_HPP_
#define _LOCAL_TYPE_TRAITS_HPP_

/**
 * @brief Type trait to check if a type is readable from an input stream.
 * @details The `is_readable` class template provides a compile-time boolean
 * value indicating whether a given type `T` is readable from an input stream.
 *
 * @tparam T The type to check for readability.
 */
template <class T>
class is_readable {
 private:
  /**
   * @brief Match if a type is readable from an input stream.
   *
   * @tparam TT The type to test for readability.
   * @param[in] Unused parameter used for SFINAE.
   * @returns std::true_type if the type is readable.
   */
  template <typename TT>
  static auto test(int)
    -> decltype(std::declval<std::istream &>() >> std::declval<TT>(), std::true_type());

  /**
   * @brief Fallback function for types that are not readable.
   *
   * @tparam Unused template parameter.
   * @param[in] Unused parameter used for SFINAE.
   * @return std::false_type indicating that the type is not readable.
   */
  template <typename>
  static auto test(...) -> std::false_type;

 public:
  /**
   * @brief The boolean value indicating if the type is readable.
   *
   * The `value` member is a compile-time constant boolean value that is `true`
   * if the type `T` is readable from an input stream, and `false` otherwise.
   */
  static constexpr bool value = decltype(test<T>(0))::value;
};

/**
 * @brief Shortcut variable to get the value of the `is_readable` type trait.
 * @details The `is_readable_v` variable template provides a simple way to check
 * if a given type `T` is readable from an input stream.
 *
 * @tparam T The type to check for readability.
 */
template <typename T>
constexpr bool is_readable_v = is_readable<T>::value;

#endif  // _LOCAL_TYPE_TRAITS_HPP_