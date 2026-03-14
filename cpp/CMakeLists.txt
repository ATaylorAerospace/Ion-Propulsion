cmake_minimum_required(VERSION 3.20)
project(ion_propulsion VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Author: A Taylor

add_library(ion_propulsion
    src/dynamics/mission_profiles.cpp
    src/propulsion/thruster.cpp
    src/optimization/solvers.cpp
)
target_include_directories(ion_propulsion PUBLIC include)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(test_dynamics tests/test_dynamics.cpp)
target_link_libraries(test_dynamics ion_propulsion GTest::gtest_main)

add_executable(test_propulsion tests/test_propulsion.cpp)
target_link_libraries(test_propulsion ion_propulsion GTest::gtest_main)

add_executable(test_optimization tests/test_optimization.cpp)
target_link_libraries(test_optimization ion_propulsion GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_dynamics)
gtest_discover_tests(test_propulsion)
gtest_discover_tests(test_optimization)
