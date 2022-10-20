cmake_minimum_required(VERSION 3.23)
project(ExternalMergeSort)

set(CMAKE_CXX_STANDARD 17)

include_directories(header src)

add_library(ExternalMergeSortlib

        ${PROJECT_SOURCE_DIR}/header/Buffer.h
        header/BufferIO.h
        header/MergeSort.h
        header/SortFunc.h
        header/LoserTree.h
        header/Generator.h

        ${PROJECT_SOURCE_DIR}/src/Buffer.cpp
        src/MergeSort.cpp
        src/BufferIO.cpp
        src/SortFunc.cpp
        src/Generator.cpp)

add_executable(main)
target_sources(main PUBLIC main.cpp)
link_libraries(main PUBLIC ExternalMergeSortlib)
