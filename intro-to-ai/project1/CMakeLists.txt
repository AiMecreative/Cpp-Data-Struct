cmake_minimum_required(VERSION 3.23)
project(project1)

set(CMAKE_CXX_STANDARD 17)
include_directories(header src)

add_library(Searchlib
        ${PROJECT_SOURCE_DIR}/header/Action.h
        ${PROJECT_SOURCE_DIR}/header/Node.h
        ${PROJECT_SOURCE_DIR}/header/Problem.h
        ${PROJECT_SOURCE_DIR}/header/Search.h
        ${PROJECT_SOURCE_DIR}/header/Heuristic.h
        ${PROJECT_SOURCE_DIR}/src/Action.cpp
        ${PROJECT_SOURCE_DIR}/src/Node.cpp
        ${PROJECT_SOURCE_DIR}/src/Problem.cpp
        ${PROJECT_SOURCE_DIR}/src/Search.cpp
        ${PROJECT_SOURCE_DIR}/src/Heuristic.cpp)
#add_library(Heuristiclib
#        ${PROJECT_SOURCE_DIR}/header/Heuristic.h
#        ${PROJECT_SOURCE_DIR}/src/Heuristic.cpp)
#add_library(Nodelib
#        ${PROJECT_SOURCE_DIR}/header/Node.h
#        ${PROJECT_SOURCE_DIR}/src/Node.cpp)
#add_library(Problemlib
#        ${PROJECT_SOURCE_DIR}/header/Problem.h
#        ${PROJECT_SOURCE_DIR}/src/Problem.cpp)
#add_library(Searchlib
#        ${PROJECT_SOURCE_DIR}/header/Search.h
#        ${PROJECT_SOURCE_DIR}/src/Search.cpp)

add_executable(main)
target_sources(main PUBLIC main.cpp)
target_link_libraries(main PUBLIC Searchlib)
