// clang-format off
/**
 * University: Universidad de La Laguna
 * Center: Escuela Superior de Ingeniería y Tecnología
 * Grade: Ingeniería Informática
 * Subject: T.F.G.
 * Course: Fifth
 * Institutional email: gian.diana.28@ull.edu.es
 *
 * @file matrix.hpp
 * @author Gian Luis Bolivar Diana
 * @version 1.0.0
 * @date April 10, 2024
 * @copyright Copyright (c) 2024
 *
 * @brief File containing the description of a bidimensional array class.
 * It implements the basic operations of a matrix like resize, access to the
 * elements, and iterators.
 *
 * @see GitHub repository: @link https://github.com/gianluisdiana/OpsCplex @endlink
 * @see Google style guide: @link https://google.github.io/styleguide/cppguide.html @endlink
 */
// clang-format on

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <iostream>
#include <utility>
#include <vector>

/**
 * @brief Represents a bidimensional array
 *
 * @tparam T The data type contained in the matrix.
 */
template <typename T>
class Matrix {
 public:
  // --------------------------- Type definitions -------------------------- //

  using difference_type = typename std::vector<T>::difference_type;
  using value_type = typename std::vector<T>::value_type;
  using pointer = typename std::vector<T>::pointer;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

  using iterator = typename std::vector<int>::iterator;
  using vector_iterator = typename std::vector<std::vector<T>>::iterator;

  /** @brief Represents an iterator for the matrix. */
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;

   private:
    vector_iterator start_, sentinel_;
    iterator ptr_, current_sentinel_;
    static_assert(std::sentinel_for<decltype(sentinel_), decltype(start_)>);
    static_assert(std::sentinel_for<
                  decltype(current_sentinel_), decltype(ptr_)>);

   public:
    Iterator() = default;

    /**
     * @brief Initializes the iterator object with the given parameters.
     *
     * @param start_iterator An iterator located at the beginning of the
     * vector.
     * @param stop_iterator An iterator located at the end of the vector.
     */
    Iterator(vector_iterator start_iterator, vector_iterator stop_iterator) :
      start_(start_iterator), sentinel_(stop_iterator),
      ptr_(start_iterator->begin()), current_sentinel_(start_iterator->end()) {}

    /** @brief Reference to the value where the iterator is located. */
    reference operator*() const {
      return *ptr_;
    }

    /** @brief Pointer to the value where the iterator is located. */
    pointer operator->() const {
      return ptr_;
    }

    /**
     * @brief Advances the iterator to the next position.
     * It will move to the next row if the current row is finished.
     *
     * @returns A reference to the iterator.
     */
    auto &operator++() {
      ++ptr_;
      if (ptr_ == current_sentinel_ && start_ != sentinel_) {
        ++start_;
        ptr_ = start_->begin();
        current_sentinel_ = start_->end();
      }
      return *this;
    }

    /**
     * @brief Advances the iterator to the next position.
     * It will move to the next row if the current row is finished.
     *
     * @returns A copy of the iterator before the increment.
     */
    auto operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    /**
     * @brief Compares the iterator with another iterator.
     * @details Automatically generated by the compiler since C++20.
     */
    auto operator<=>(const Iterator &other) const = default;

    /**
     * @brief Returns an iterator pointing to the first element of the matrix.
     */
    auto begin() {
      return start_->begin();
    }

    /**
     * @brief Returns an iterator pointing to the last element of the matrix.
     */
    auto end() {
      return sentinel_->end();
    }
  };

  // ----------------------------- Constructors ---------------------------- //

  /**
   * @brief Empty constructor, only reserve the size of the matrix
   *
   * @param size The size of the matrix. The first element is the amount of rows
   * and the second element is the amount of columns.
   */
  explicit Matrix(std::pair<std::size_t, std::size_t> size = {1, 1});

  /**
   * @brief Initialize the matrix with an bidimensional array of the type.
   *
   * @tparam rows_amount The amount of rows the 2D array has.
   * @tparam cols_amount The amount of cols the 2D array has.
   * @param matrix The bidimensional array to get the data from.
   */
  template <std::size_t rows_amount, std::size_t cols_amount>
  explicit Matrix(T (&matrix)[rows_amount][cols_amount]);

  /** @brief Returns the data stored in the matrix. */
  [[nodiscard]] const std::vector<std::vector<T>> &data() const {
    return data_;
  }

  // ------------------------------- Capacity ------------------------------ //

  /** @brief Checks if the matrix is empty. */
  [[nodiscard]] bool empty() const {
    return data_.empty() || data_[0].empty();
  }

  /**
   * @brief Returns the size of the matrix.
   *
   * @return A pair containing the number of rows and columns in the matrix.
   */
  [[nodiscard]] std::pair<std::size_t, std::size_t> size() const {
    return {rows_amount_, cols_amount_};
  }

  // ---------------------------- Size methods ----------------------------- //

  /**
   * @brief Resize the matrix with the given amount of rows and columns.
   *
   * @param size The new size of the matrix. The first element is the amount of
   * rows and the second element is the amount of columns.
   */
  void resize(std::pair<std::size_t, std::size_t> size);

  /**
   * @brief Resize the matrix with the given size, making it a square matrix.
   *
   * @param size The new size of the matrix.
   */
  void resize(std::size_t size);

  // --------------------------- Data management --------------------------- //

  /**
   * @brief Initialize the matrix with the given data.
   *
   * @param data The data to initialize the matrix with.
   */
  void init(const T data) {
    for (auto &row : data_) { row.assign(cols_amount_, data); }
  }

  // ----------------------------- Iterators ------------------------------ //

  /**
   * @brief Get the begin iterator of the matrix.
   *
   * @returns The begin iterator of the matrix.
   */
  [[nodiscard]] Iterator begin() {
    return Iterator(data_.begin(), data_.end());
  }

  /**
   * @brief Get the end iterator of the matrix.
   *
   * @returns The end iterator of the matrix.
   */
  [[nodiscard]] Iterator end() {
    return Iterator(data_.end(), data_.end());
  }

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the [] operator to give write access to the row in the
   * given index.
   *
   * @param index The index of the row to access.
   * @returns The row selected of the matrix
   */
  [[nodiscard]] std::vector<T> &operator[](const std::size_t index) {
    return data_[index];
  }

  /**
   * @brief Overload of the [] operator to give read access to the row in the
   * given index.
   *
   * @param index The index of the row to access.
   * @returns The row selected of the matrix
   */
  [[nodiscard]] const std::vector<T> &
  operator[](const std::size_t index) const {
    return data_[index];
  }

  /**
   * @brief Overload of the () operator to give write access to the element in
   * the given row and column index.
   *
   * @param coords The pair containing the row and column index (in that order)
   * to access.
   * @returns The element placed in the given row and column index.
   */
  [[nodiscard]] reference
  operator()(const std::pair<std::size_t, std::size_t> coords) {
    return data_[coords.first][coords.second];
  }

  /**
   * @brief Overload of the () operator to give read access to the element in
   * the given row and column index.
   *
   * @param coords The pair containing the row and column index (in that order)
   * to access.
   * @returns The element placed in the given row and column index.
   */
  [[nodiscard]] const_reference
  operator()(const std::pair<std::size_t, std::size_t> coords) const {
    return data_[coords.first][coords.second];
  }

  /**
   * @brief Overload of the = operator to assign the data of the given matrix to
   * the current matrix.
   *
   * @param matrix The matrix to assign the data from.
   * @returns The matrix with the data assigned.
   */
  Matrix &operator=(const std::vector<std::vector<T>> &matrix);

  /**
   * @brief Overload of the << operator to print the matrix.
   *
   * @param out_stream Represents the outflow.
   * @param matrix The matrix to print.
   * @tparam C The data type stored in the matrix.
   * @return The outflow with the matrix formatted.
   */
  template <typename C>
  friend std::ostream &
  operator<<(std::ostream &out_stream, const Matrix<C> &matrix);

 private:
  // The amount of rows the matrix has
  std::size_t rows_amount_;
  // The amount of columns the matrix has
  std::size_t cols_amount_;
  // The data stored in the matrix
  std::vector<std::vector<T>> data_;

  /**
   * @brief Resize the matrix with the amount of rows and columns in the class.
   */
  void resize();
};

template <typename T>
Matrix<T>::Matrix(const std::pair<std::size_t, std::size_t> size) :
  rows_amount_ {size.first}, cols_amount_ {size.second} {
  resize();
}

template <typename T>
template <std::size_t rows_amount, std::size_t cols_amount>
Matrix<T>::Matrix(T (&matrix)[rows_amount][cols_amount]) :
  rows_amount_ {rows_amount}, cols_amount_ {cols_amount}, data_(rows_amount) {
  for (std::size_t idx = 0; idx < rows_amount_; ++idx) {
    data_[idx].assign(matrix[idx], matrix[idx] + cols_amount_);
  }
}

template <typename T>
void Matrix<T>::resize(const std::pair<std::size_t, std::size_t> size) {
  std::tie(rows_amount_, cols_amount_) = size;
  resize();
}

template <typename T>
void Matrix<T>::resize(const std::size_t size) {
  rows_amount_ = cols_amount_ = size;
  resize();
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const std::vector<std::vector<T>> &matrix) {
  data_ = matrix;
  rows_amount_ = matrix.size();
  cols_amount_ = matrix[0].size();
  return *this;
}

template <typename C>
std::ostream &operator<<(std::ostream &out_stream, const Matrix<C> &matrix) {
  out_stream << "[\n";
  for (const auto &row : matrix.data_) {
    out_stream << "\t[";
    for (std::size_t idx = 0; idx < row.size() - 1; ++idx) {
      out_stream << row[idx] << ", ";
    }
    out_stream << row[row.size() - 1] << "]\n";
  }
  return out_stream << "]";
}

template <typename T>
void Matrix<T>::resize() {
  data_.resize(rows_amount_);
  for (auto &row : data_) { row.resize(cols_amount_); }
}

#endif  // MATRIX_HPP_