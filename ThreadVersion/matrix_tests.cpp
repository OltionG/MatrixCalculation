#include "Matrix.h"
#include <iostream>
#include <cassert>
#include <chrono>

void test_constructor() {
    std::cout << "--- Testing Constructors ---" << std::endl;
    
    // Test the constructor that takes dimensions.
    Matrix<int> m(2, 3);
    assert(m.getRows() == 2 && m.getCols() == 3);
    std::cout << "Default constructor passed for (2, 3)." << std::endl;

    // Test a 1x1 matrix constructor
    Matrix<int> m1x1(1, 1);
    assert(m1x1.getRows() == 1 && m1x1.getCols() == 1);
    std::cout << "Default constructor passed for (1, 1)." << std::endl;

    // Test the constructor that takes an initializer list.
    Matrix<int> m2 = {{1, 2, 3}, {4, 5, 6}};
    assert(m2.getRows() == 2 && m2.getCols() == 3);
    assert(m2(0, 0) == 1 && m2(1, 2) == 6);
    std::cout << "Initializer-list constructor passed." << std::endl;
    std::cout << "Matrix created with initializer list:" << std::endl;
    std::cout << m2 << std::endl;

    // Test for exceptions on invalid input (zero dimension).
    try {
        Matrix<int> m3(0, 5);
        assert(false); // This line should not be reached.
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for zero dimension: " << e.what() << std::endl;
    }
    // Test for exceptions on invalid input (differing row lengths).
    try {
        Matrix<int> m4 = {{1, 2}, {3}};
        assert(false); // This line should not be reached.
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for differing row lengths: " << e.what() << std::endl;
    }
}

void test_addition() {
    std::cout << "\n--- Testing Addition ---" << std::endl;
    
    // Positive numbers
    Matrix<int> m1 = {{1, 2}, {3, 4}};
    Matrix<int> m2 = {{5, 6}, {7, 8}};
    Matrix<int> m3 = m1 + m2;
    Matrix<int> expected_sum = {{6, 8}, {10, 12}};
    assert(m3(0, 0) == expected_sum(0, 0) && m3(1, 1) == expected_sum(1, 1));
    std::cout << m1 << " + " << std::endl << m2 << " = " << std::endl << m3 << std::endl;
    
    // Negative numbers and zeros
    Matrix<int> m4 = {{-1, 0}, {-5, -10}};
    Matrix<int> m5 = {{1, 1}, {5, 10}};
    Matrix<int> m6 = m4 + m5;
    Matrix<int> expected_sum_2 = {{0, 1}, {0, 0}};
    assert(m6(0, 0) == expected_sum_2(0, 0) && m6(1, 1) == expected_sum_2(1, 1));
    std::cout << m4 << " + " << std::endl << m5 << " = " << std::endl << m6 << std::endl;
    
    // Exception for incompatible dimensions
    try {
        Matrix<int> m_small = {{1}};
        m1 + m_small;
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for addition with incompatible dimensions: " << e.what() << std::endl;
    }
}

void test_subtraction() {
    std::cout << "\n--- Testing Subtraction ---" << std::endl;
    
    // Positive numbers
    Matrix<int> m1 = {{5, 6}, {7, 8}};
    Matrix<int> m2 = {{1, 2}, {3, 4}};
    Matrix<int> m3 = m1 - m2;
    Matrix<int> expected_diff = {{4, 4}, {4, 4}};
    assert(m3(0, 0) == expected_diff(0, 0) && m3(1, 1) == expected_diff(1, 1));
    std::cout << m1 << " - " << std::endl << m2 << " = " << std::endl << m3 << std::endl;
    
    // Negative numbers and zeros
    Matrix<int> m4 = {{10, 5}, {0, -5}};
    Matrix<int> m5 = {{5, 5}, {0, 5}};
    Matrix<int> m6 = m4 - m5;
    Matrix<int> expected_diff_2 = {{5, 0}, {0, -10}};
    assert(m6(0, 0) == expected_diff_2(0, 0) && m6(1, 1) == expected_diff_2(1, 1));
    std::cout << m4 << " - " << std::endl << m5 << " = " << std::endl << m6 << std::endl;
}

void test_multiplication() {
    std::cout << "\n--- Testing Multiplication ---" << std::endl;
    
    // Standard multiplication with small matrices
    Matrix<int> m1 = {{1, 2}, {3, 4}};
    Matrix<int> m2 = {{5, 6}, {7, 8}};
    Matrix<int> m3 = m1 * m2;
    Matrix<int> expected_product = {{19, 22}, {43, 50}};
    assert(m3(0, 0) == expected_product(0, 0) && m3(1, 1) == expected_product(1, 1));
    std::cout << m1 << " * " << std::endl << m2 << " = " << std::endl << m3 << std::endl;
    
    // Performance check for a larger matrix multiplication
    const size_t N = 1000;
    Matrix<double> large_m1(N, N, 1.0);
    Matrix<double> large_m2(N, N, 2.0);

    std::cout << "Starting performance test for " << N << "x" << N << " matrix multiplication..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    Matrix<double> large_result = large_m1 * large_m2;
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    std::cout << "Multiplication took " << duration.count() << " seconds." << std::endl;

    // Check a single value to ensure the result is correct
    assert(large_result(0, 0) == 2.0 * N);
    std::cout << "Large matrix multiplication test passed." << std::endl;
    
    // Multiplication with a 1x1 matrix
    Matrix<int> m_1x1 = {{10}};
    Matrix<int> m_vec = {{1, 2, 3}};
    Matrix<int> m_result = m_1x1 * m_vec;
    Matrix<int> expected_result = {{10, 20, 30}};
    assert(m_result(0, 0) == expected_result(0, 0) && m_result(0, 2) == expected_result(0, 2));
    std::cout << m_1x1 << " * " << std::endl << m_vec << " = " << std::endl << m_result << std::endl;

    // Multiplication with negative numbers
    Matrix<int> m4 = {{-1, 2}, {3, -4}};
    Matrix<int> m5 = {{1, -1}, {-2, 3}};
    Matrix<int> m6 = m4 * m5;
    Matrix<int> expected_product_2 = {{-5, 7}, {11, -15}};
    assert(m6(0, 0) == expected_product_2(0, 0) && m6(1, 1) == expected_product_2(1, 1));
    std::cout << m4 << " * " << std::endl << m5 << " = " << std::endl << m6 << std::endl;

    // Exception for incompatible dimensions
    try {
        Matrix<int> m_invalid = {{1}, {2}, {3}};
        m1 * m_invalid;
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for multiplication with incompatible dimensions: " << e.what() << std::endl;
    }
}

void test_transpose() {
    std::cout << "\n--- Testing Transpose ---" << std::endl;
    
    // Standard 2x3 matrix
    Matrix<int> m1 = {{1, 2, 3}, {4, 5, 6}};
    Matrix<int> m2 = m1.transpose();
    Matrix<int> expected_transpose = {{1, 4}, {2, 5}, {3, 6}};
    assert(m2.getRows() == 3 && m2.getCols() == 2);
    assert(m2(0, 1) == expected_transpose(0, 1) && m2(2, 1) == expected_transpose(2, 1));
    std::cout << "Original Matrix:" << std::endl << m1 << std::endl;
    std::cout << "Transposed Matrix:" << std::endl << m2 << std::endl;
    
    // Square matrix
    Matrix<int> m3 = {{1, 2}, {3, 4}};
    Matrix<int> m4 = m3.transpose();
    Matrix<int> expected_transpose_2 = {{1, 3}, {2, 4}};
    assert(m4(0, 1) == expected_transpose_2(0, 1) && m4(1, 0) == expected_transpose_2(1, 0));
    std::cout << "Original Square Matrix:" << std::endl << m3 << std::endl;
    std::cout << "Transposed Square Matrix:" << std::endl << m4 << std::endl;
}

void test_edge_cases() {
    std::cout << "\n--- Testing Edge Cases ---" << std::endl;

    // Test operations on a 1x1 matrix
    Matrix<int> m1 = {{5}};
    Matrix<int> m2 = {{3}};
    Matrix<int> sum = m1 + m2;
    Matrix<int> diff = m1 - m2;
    Matrix<int> prod = m1 * m2;
    
    assert(sum(0, 0) == 8);
    assert(diff(0, 0) == 2);
    assert(prod(0, 0) == 15);
    std::cout << "1x1 Matrix tests passed." << std::endl;

    // Test a matrix of all zeros
    Matrix<int> zero_m = {{0, 0}, {0, 0}};
    Matrix<int> m_with_zeros = {{1, 2}, {3, 4}};
    Matrix<int> zero_sum = zero_m + m_with_zeros;
    
    assert(zero_sum(0, 0) == 1 && zero_sum(1, 1) == 4);
    std::cout << "Zero matrix tests passed." << std::endl;
}

void test_scalar_multiplication() {
    std::cout << "\n--- Testing Scalar Multiplication ---" << std::endl;
    Matrix<int> m = {{1, 2}, {3, 4}};
    Matrix<int> result = m * 2;
    Matrix<int> expected_result = {{2, 4}, {6, 8}};
    assert(result(0, 0) == expected_result(0, 0) && result(1, 1) == expected_result(1, 1));
    std::cout << m << " * 2 = " << std::endl << result << std::endl;
    std::cout << "Scalar multiplication test passed." << std::endl;
}

void test_out_of_bounds() {
    std::cout << "\n--- Testing Out of Bounds Access ---" << std::endl;
    Matrix<int> m(2, 2);

#ifdef NDEBUG
    try {
        m(2, 0); // Out of bounds row access
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception for out of bounds row access: " << e.what() << std::endl;
    }
    try {
        m(0, 2); // Out of bounds col access
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception for out of bounds column access: " << e.what() << std::endl;
    }
#else
    std::cout << "Running in debug mode. Expecting assertions for out of bounds access." << std::endl;
    // The next two lines would cause a program termination via assert, so we're commenting them out
    // to allow the test suite to run to completion. You can uncomment them to test assert.
    // m(2, 0);
    // m(0, 2);
    std::cout << "Bounds checks would have triggered asserts. Test for this is skipped." << std::endl;
#endif
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    test_constructor();
    test_addition();
    test_subtraction();
    test_multiplication();
    test_transpose();
    test_edge_cases();
    test_scalar_multiplication();
    test_out_of_bounds();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_duration = end_time - start_time;

    std::cout << "\nAll tests passed!" << std::endl;
    std::cout << "Total program execution time: " << total_duration.count() << " seconds." << std::endl;

    return 0;
}