cmake_minimum_required(VERSION 3.23)
project(ExternalQuickSort)

set(CMAKE_CXX_STANDARD 17)

include_directories(header src)

add_library(ExternalQSlib
        ${PROJECT_SOURCE_DIR}/header/ExternalQS.h
        ${PROJECT_SOURCE_DIR}/src/ExternalQS.cpp)

add_library(PriorityDequelib
        ${PROJECT_SOURCE_DIR}/header/PriorityDeque.h
        ${PROJECT_SOURCE_DIR}/src/PriorityDeque.cpp)

add_executable(test)
target_sources(test PUBLIC test.cpp)
target_link_libraries(test PUBLIC ExternalQSlib PriorityDequelib)
target_compile_definitions(test PUBLIC -DASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")