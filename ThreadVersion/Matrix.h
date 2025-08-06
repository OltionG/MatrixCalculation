#pragma once

#include <vector>
#include <stdexcept>
#include <initializer_list>
#include <cassert>
#include <ostream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

template <typename T>
class Matrix {
private:
    std::vector<T> data;
    size_t rows;
    size_t cols;

public:
    // Combined constructor
    Matrix(size_t num_rows, size_t num_cols, const T& default_value = T{})
        : rows(num_rows), cols(num_cols) {
        if (num_rows == 0 || num_cols == 0) {
            throw std::invalid_argument("Matrix dimensions cannot be zero.");
        }
        if (num_rows > 0 && num_cols > std::numeric_limits<size_t>::max() / num_rows) {
            throw std::length_error("Matrix size exceeds maximum capacity.");
        }
        data.resize(rows * cols, default_value);
    }

    // Existing constructor for initializer list
    Matrix(std::initializer_list<std::initializer_list<T>> list) {
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
    
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    
    T& operator()(size_t row, size_t col) {
        assert(row < rows && col < cols && "Matrix element access out of bounds.");
        return data[row * cols + col];
    }
    const T& operator()(size_t row, size_t col) const {
        assert(row < rows && col < cols && "Matrix element access out of bounds.");
        return data[row * cols + col];
    }
    
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same dimensions for addition.");
        }
        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows * cols; ++i) {
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }
    
    Matrix<T> operator-(const Matrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same dimensions for subtraction.");
        }
        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows * cols; ++i) {
            result.data[i] = this->data[i] - other.data[i];
        }
        return result;
    }

    Matrix<T> operator*(const Matrix<T>& other) const;

    // New scalar multiplication operator
    Matrix<T> operator*(const T& scalar) const {
        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows * cols; ++i) {
            result.data[i] = this->data[i] * scalar;
        }
        return result;
    }

    Matrix<T> transpose() const {
        Matrix<T> result(cols, rows);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }
    
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix);

private:
    void multiply_rows(Matrix<T>& result, const Matrix<T>& other, size_t start_row, size_t end_row) const {
        for (size_t i = start_row; i < end_row; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                T sum{};
                for (size_t k = 0; k < cols; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
    }
};

// Friend function definition
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            os << std::setw(8) << matrix(i, j);
        }
        os << std::endl;
    }
    return os;
}

// Parallel multiplication implementation
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix for multiplication.");
    }
    Matrix<T> result(rows, other.cols);
    
    const size_t num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads for multiplication." << std::endl;
    if (num_threads == 0) { // Fallback for systems that can't detect hardware concurrency
        multiply_rows(result, other, 0, rows);
        return result;
    }
    
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    
    const size_t rows_per_thread = rows / num_threads;
    size_t start_row = 0;
    
    for (size_t i = 0; i < num_threads; ++i) {
        size_t end_row = start_row + rows_per_thread;
        if (i == num_threads - 1) {
            end_row = rows; // Ensure the last thread handles any remaining rows
        }
        
        threads.emplace_back([this, &result, &other, start_row, end_row]() {
            for (size_t i = start_row; i < end_row; ++i) {
                for (size_t j = 0; j < other.cols; ++j) {
                    T sum{};
                    for (size_t k = 0; k < cols; ++k) {
                        sum += (*this)(i, k) * other(k, j);
                    }
                    result(i, j) = sum;
                }
            }
        });
        start_row = end_row;
    }

    for (auto& thread : threads) {
        thread.join();
    }
    
    return result;
}