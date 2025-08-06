cmake_minimum_required(VERSION 3.10)

project(MatrixLibrary CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matrix_tests matrix_tests.cpp)

target_include_directories(matrix_tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")