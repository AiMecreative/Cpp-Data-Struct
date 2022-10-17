cmake_minimum_required(VERSION 3.23)
project(MatrixMultiply)

set(CMAKE_CXX_STANDARD 17)

include_directories(header src)

add_library(MatrixMultiplylib
        ${PROJECT_SOURCE_DIR}/header/Matrix.h header/MatrixIO.h header/MatrixMultiplier.h
        ${PROJECT_SOURCE_DIR}/src/Matrix.cpp src/MatrixIO.cpp src/MatrixMultiplier.cpp)

add_executable(main header/MatrixMultiplier.h header/Order.h)
target_sources(main PUBLIC main.cpp)
target_link_libraries(main PUBLIC MatrixMultiplylib)
#target_compile_definitions(main PUBLIC -DASSET_DIR="${PROJECT_SOURCE_DIR}/data/")
