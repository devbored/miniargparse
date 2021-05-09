cmake_minimum_required(VERSION 3.10)

project(miniargparse_tests)

set(GTEST_BASEDIR "")
find_library(GTEST gtest HINTS ${GTEST_BASEDIR} REQUIRED)
find_library(GTEST_MAIN gtest_main HINTS ${GTEST_BASEDIR} REQUIRED)

find_package(Threads)

include_directories(${CMAKE_SOURCE_DIR})

add_compile_options(-Wall)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(miniargparse_tests test_basic.cpp)
target_link_libraries(miniargparse_tests ${GTEST} ${GTEST_MAIN} ${CMAKE_THREAD_LIBS_INIT})