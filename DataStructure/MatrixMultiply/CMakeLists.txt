cmake_minimum_required(VERSION 3.23)
project(MatrixMultiply)

set(CMAKE_CXX_STANDARD 17)

add_library(MatrixMultiplylib
        ${PROJECT_SOURCE_DIR}/header/MatrixMultiply.h
        ${PROJECT_SOURCE_DIR}/src/MatrixMultiply.cpp)

add_executable(main)
target_sources(main PUBLIC main.cpp)
target_link_libraries(main PUBLIC MatrixMultiplylib)