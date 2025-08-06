#include "Matrix.h"
#include <iomanip>
#include <iostream>

template <typename T>
Matrix<T>::Matrix(size_t num_rows, size_t num_cols, const T& default_value) : rows(num_rows), cols(num_cols) {
    if (num_rows == 0 || num_cols == 0) {
        throw std::invalid_argument("Matrix dimensions cannot be zero.");
    }
    data.resize(rows * cols, T{});
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> list) {
    rows = list.size();
    if (rows == 0) {
        cols = 0;
        return;
    }
    cols = list.begin()->size();
    if (cols == 0) {
        throw std::invalid_argument("Row length cannot be zero.");
    }
    
    data.reserve(rows * cols);
    for (const auto& row_list : list) {
        if (row_list.size() != cols) {
            throw std::invalid_argument("Rows have differing lengths.");
        }
        data.insert(data.end(), row_list.begin(), row_list.end());
    }
}

template <typename T>
T& Matrix<T>::operator()(size_t row, size_t col) {
    assert(row < rows && col < cols);
    return data[row * cols + col];
}

template <typename T>
const T& Matrix<T>::operator()(size_t row, size_t col) const {
    assert(row < rows && col < cols);
    return data[row * cols + col];
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same dimensions for addition.");
    }
    Matrix<T> result(rows, cols);
    for (size_t i = 0; i < rows * cols; ++i) {
        result.data[i] = this->data[i] + other.data[i];
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same dimensions for subtraction.");
    }
    Matrix<T> result(rows, cols);
    for (size_t i = 0; i < rows * cols; ++i) {
        result.data[i] = this->data[i] - other.data[i];
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix for multiplication.");
    }
    Matrix<T> result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < other.cols; ++j) {
            T sum{};
            for (size_t k = 0; k < cols; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> result(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix) {
    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            os << std::setw(8) << matrix(i, j);
        }
        os << std::endl;
    }
    return os;
}

// Explicit template instantiations for common types
template class Matrix<int>;
template class Matrix<double>;
template std::ostream& operator<<(std::ostream&, const Matrix<int>&);
template std::ostream& operator<<(std::ostream&, const Matrix<double>&);