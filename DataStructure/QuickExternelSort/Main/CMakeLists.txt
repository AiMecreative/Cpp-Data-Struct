cmake_minimum_required(VERSION 3.0.0)
project(QuickExternalSort VERSION 0.1.0)

add_executable(QuickExternalSort)
file(GLOB_RECURSE sources CONFIGURE_DEPENDES *.cpp *.h)
target_sources(testHeap PUBLIC ${sources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
