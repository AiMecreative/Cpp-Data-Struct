cmake_minimum_required(VERSION 3.23)
project(ExternalQuickSort)

set(CMAKE_CXX_STANDARD 14)

link_directories(header src)
include_directories(header src)
file(GLOB sources
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/header/*.h)

add_executable(test)
target_sources(test PUBLIC ${sources})