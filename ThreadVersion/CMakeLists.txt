# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(MatrixProject CXX)

# Add an executable called 'matrix_tests' from a source file
add_executable(matrix_tests matrix_tests.cpp)

# Find the Threads package, which is required for std::thread
find_package(Threads REQUIRED)

# Link the executable with the Threads library
target_link_libraries(matrix_tests Threads::Threads)

# Set a C++ standard (e.g., C++17)
set_property(TARGET matrix_tests PROPERTY CXX_STANDARD 17)
set_property(TARGET matrix_tests PROPERTY CXX_STANDARD_REQUIRED ON)