cmake_minimum_required(VERSION 3.17)
project(ExternalMergeSort-v2)

set(CMAKE_CXX_STANDARD 17)

include_directories(header src)

add_library(ExternalMergeSortlib
        ${PROJECT_SOURCE_DIR}/header/Buffer.hpp
        header/MergeSort.hpp
        header/SortFunc.hpp
        header/Generator.hpp
        header/LoserTree.hpp
        src/Buffer.cpp
        src/SortFunc.cpp
        src/Generator.cpp
        src/MergeFunc.cpp
        src/LoserTree.cpp header/SorterWrapper.hpp)

add_executable(main)
target_sources(main PUBLIC main.cpp)
target_link_libraries(main PUBLIC ExternalMergeSortlib)

target_compile_definitions(main PUBLIC -DDATA_DIR="${PROJECT_SOURCE_DIR}")