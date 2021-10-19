cmake_minimum_required(VERSION 3.10)

project(miniargparse_tests)

# External dependencies
find_package(GTest CONFIG REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SRCS *.cpp)
add_executable(miniargparse_tests ${SRCS})
target_include_directories(miniargparse_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(miniargparse_tests PRIVATE "--std=c++11")
if (MSVC)
    target_compile_options(miniargparse_tests PRIVATE "/W4")
else()
    target_compile_options(miniargparse_tests PRIVATE -Wall)
    target_compile_options(miniargparse_tests PRIVATE -pedantic)
    target_compile_options(miniargparse_tests PRIVATE -Wno-unused-function)
endif()
target_link_libraries(miniargparse_tests
    GTest::gtest
    GTest::gtest_main
)
