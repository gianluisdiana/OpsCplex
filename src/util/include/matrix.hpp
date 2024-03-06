/**
 * @author Gian Luis Bolivar Diana (gianluisbolivar1@gmail.com)
 * @date November 7, 2022
 *
 * @file matrix.hpp
 * @version 0.2
 * @brief
 *
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <iostream>
#include <vector>

#include <json_interface.hpp>

/**
 * @brief Represents a bidimensional array
 *
 * @tparam T The data type contained in the matrix.
 */
template <typename T>
class Matrix : public JsonInterface {
 public:
  // ----------------------------- Constructors ----------------------------- //
  /**
   * @brief Empty constructor, only reserve the size of the matrix
   *
   * @param rows_amount The amount of rows the matrix will have
   * @param cols_amount The amount of columns the matrix will have
   */
  Matrix(const std::size_t rows_amount = 1, const std::size_t cols_amount = 1);

  /**
   * @brief Initialize the matrix with an bidimensional array of the type.
   *
   * @tparam rows_amount The amount of rows the 2D array has.
   * @tparam cols_amount The amount of cols the 2D array has.
   * @param matrix The bidimensional array to get the data from.
   */
  template <std::size_t rows_amount, std::size_t cols_amount>
  Matrix(T (&matrix)[rows_amount][cols_amount]);

  // ------------------------------ Getters ------------------------------- //

  /**
   * @brief Get the amount of rows the matrix has.
   *
   * @returns The amount of rows the matrix has.
   */
  inline std::size_t getRowsAmount() const {
    return rows_amount_;
  }

  /**
   * @brief Get the amount of columns the matrix has.
   *
   * @returns The amount of columns the matrix has.
   */
  inline std::size_t getColsAmount() const {
    return cols_amount_;
  }

  // ---------------------------- Size methods ----------------------------- //

  /**
   * @brief Resize the matrix with the given amount of rows and columns.
   *
   * @param rows_amount The amount of rows the matrix will have.
   * @param cols_amount The amount of columns the matrix will have.
   */
  void resize(const std::size_t rows_amount, const std::size_t cols_amount) {
    rows_amount_ = rows_amount;
    cols_amount_ = cols_amount;
    resize();
  }

  // --------------------------- Data management --------------------------- //

  /**
   * @brief Initialize the matrix with the given data.
   *
   * @param data The data to initialize the matrix with.
   */
  void init(const T data) {
    for (auto &row : data_) {
      for (auto &value : row) value = data;
    }
  }

  // ------------------------------ Operators ------------------------------ //

  /**
   * @brief Overload of the [] operator to give write access to the row in the
   * given index.
   *
   * @param index The index of the row to access.
   * @returns The row selected of the matrix
   */
  inline std::vector<T> &operator[](const std::size_t index) {
    return data_[index];
  }

  /**
   * @brief Overload of the [] operator to give read access to the row in the
   * given index.
   *
   * @param index The index of the row to access.
   * @returns The row selected of the matrix
   */
  inline const std::vector<T> &operator[](const std::size_t index) const {
    return data_[index];
  }

  /**
   * @brief Overload of the () operator to give write access to the element in
   * the given row and column index.
   *
   * @param row_index The index of the row to access.
   * @param col_index The index of the col to access.
   * @returns The element placed in the given row and column index.
   */
  inline typename std::vector<T>::reference
  operator()(const std::size_t row_index, const std::size_t col_index) {
    return data_[row_index][col_index];
  }

  /**
   * @brief Overload of the () operator to give read access to the element in
   * the given row and column index.
   *
   * @param row_index The index of the row to access.
   * @param col_index The index of the col to access.
   * @returns The element placed in the given row and column index.
   */
  inline typename std::vector<T>::const_reference
  operator()(const std::size_t row_index, const std::size_t col_index) const {
    return data_[row_index][col_index];
  }

  /**
   * @brief Overload of the << operator to print the matrix.
   *
   * @param os Represents the outflow.
   * @param matrix The matrix to print.
   * @tparam C The data type stored in the matrix.
   * @return The outflow with the matrix formatted.
   */
  template <typename C>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<C> &matrix);

  // ----------------------------- Json methods ---------------------------- //

  /**
   * @brief Get the json representation of the matrix.
   */
  inline const nlohmann::json toJson() const override {
    return nlohmann::json(data_);
  }

  /**
   * @brief Set the matrix from a json representation.
   *
   * @param json_object The json representation of the matrix.
   */
  void setFromJson(const nlohmann::json &json_object) override;

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
Matrix<T>::Matrix(
  const std::size_t rows_amount, const std::size_t cols_amount
) :
  rows_amount_(rows_amount),
  cols_amount_(cols_amount), data_(rows_amount) {
  resize();
}

template <typename T>
template <std::size_t rows_amount, std::size_t cols_amount>
Matrix<T>::Matrix(T (&matrix)[rows_amount][cols_amount]) :
  rows_amount_(rows_amount), cols_amount_(cols_amount), data_(rows_amount) {
  for (std::size_t idx = 0; idx < rows_amount_; ++idx) {
    data_[idx].assign(matrix[idx], matrix[idx] + cols_amount_);
  }
}

template <typename T>
void Matrix<T>::resize() {
  data_.resize(rows_amount_);
  for (auto &row : data_) row.resize(cols_amount_);
}

template <typename T>
void Matrix<T>::setFromJson(const nlohmann::json &json_object) {
  data_ = json_object.get<std::vector<std::vector<T>>>();
  rows_amount_ = data_.size();
  cols_amount_ = data_[0].size();
}

template <typename C>
std::ostream &operator<<(std::ostream &os, const Matrix<C> &matrix) {
  os << "[\n";
  for (const auto &row : matrix.data_) {
    os << "\t[";
    for (std::size_t idx = 0; idx < row.size() - 1; ++idx)
      os << row[idx] << ", ";
    os << row[row.size() - 1] << "]\n";
  }
  os << "]";
  return os;
}

#endif  // _MATRIX_HPP_