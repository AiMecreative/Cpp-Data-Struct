cmake_minimum_required(VERSION 3.23)
project(ExternalQuickSort)

set(CMAKE_CXX_STANDARD 14)

link_directories(header src)

add_executable(ExternalQuickSort main.cpp)
