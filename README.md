# C++ Matrix Library

A templated C++ library for fundamental matrix operations.

## Features

  * Templated: Works with int, double, float, etc.

  * Core Operations: Addition (+), Subtraction (-), Multiplication (*, normal and parallelized), Scalar Multiplication (*), Transposition (transpose()).

  * Efficient Storage: Row-major order in std::vector for optimal cache performance.

  * Robust Error Handling: std::invalid_argument for invalid dimensions/mismatches. assert for debug bounds checks, std::out_of_range exceptions for release bounds checks.

  * Stream Output: Human-readable matrix printing via operator<<.

## Design Choices

  * Data Representation: Contiguous std::vector for cache locality.

  * Constructors: Support explicit dimensions and std::initializer_list with validation.

  * Operator Overloading: Intuitive syntax for arithmetic operations and element access.

  * Bounds Checking: assert in debug for fail-fast debugging; exceptions in release for graceful error handling and no performance overhead.

  * Parallel Multiplication: Uses std::thread to distribute row computations across CPU cores, accelerating large matrix products.

## Complexity Analysis

Let *R1​,C1*​ be dimensions of Matrix 1; *R2​,C2*​ for Matrix 2.

  * Addition/Subtraction/Scalar Mult./Transpose: *O(R×C)*

  * Matrix Multiplication: *O(R1​×C1​×C2​)* (or *O(N3)* for N×N matrices)

**Parallel Multiplication Performance**

Achieves significant **wall-clock time** speedup for large matrices by distributing work across P cores (up to P times faster). Overhead from thread management can make it slower for small matrices. Scalability improves with larger problem sizes.

## Build & Test Instructions

This project uses CMake.

  1. Clone: `git clone <repository_url> & cd MatrixLibrary`

  2. Build Dir: `mkdir build & cd build`

  3. Configure CMake:

     * Debug: `cmake -DCMAKE_BUILD_TYPE=Debug ..`

     * Release: `cmake -DCMAKE_BUILD_TYPE=Release ..`

  4. Build: `cmake --build .`

  5. Run Tests: `./matrix_tests` (from `build` directory)

The test output will show detailed results and performance metrics.
