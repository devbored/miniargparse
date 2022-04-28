cmake_minimum_required(VERSION 3.10)
project(miniargparse)

# GoogleTest
set(CMAKE_CXX_STANDARD 11)
find_package(GTest REQUIRED)

set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_executable(miniargparse_tests)
target_sources(miniargparse_tests PRIVATE
    ${TESTS_DIR}/test_bad_args.cpp
    ${TESTS_DIR}/test_good_args.cpp
)
target_include_directories(miniargparse_tests PRIVATE ${CMAKE_SOURCE_DIR})
if (MSVC)
    target_compile_options(miniargparse_tests PRIVATE "/Wall")
else()
    target_compile_options(miniargparse_tests PRIVATE "-Wall")
    target_compile_options(miniargparse_tests PRIVATE "-pedantic")
    # Certain tests dont use all of the stub's functions - silence this
    target_compile_options(miniargparse_tests PRIVATE "-Wno-unused-function")
endif()
target_link_libraries(miniargparse_tests
    GTest::GTest
    GTest::Main
)