# Author: A Taylor | Purpose: CMake build for the C++20 ion propulsion library and tests | Ref: Goebel/Katz/Vallado
cmake_minimum_required(VERSION 3.20)
project(ion_propulsion VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ION_PROPULSION_BUILD_TESTS "Build the Google Test suite" ON)

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------
add_library(ion_propulsion
    src/dynamics/mission_profiles.cpp
    src/propulsion/thruster.cpp
    src/optimization/solvers.cpp
)
add_library(ion_propulsion::ion_propulsion ALIAS ion_propulsion)

target_include_directories(ion_propulsion PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ion_propulsion PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(ion_propulsion PRIVATE /W4)
else()
    target_compile_options(ion_propulsion PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# Tests (Google Test)
# ---------------------------------------------------------------------------
if(ION_PROPULSION_BUILD_TESTS)
    enable_testing()

    # Prefer a system-installed GTest; otherwise fetch a pinned release over git
    # (works behind proxies that block archive downloads).
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.14.0
            GIT_SHALLOW    TRUE
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
        set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    include(GoogleTest)
    foreach(suite IN ITEMS dynamics propulsion optimization)
        add_executable(test_${suite} tests/test_${suite}.cpp)
        target_link_libraries(test_${suite} PRIVATE ion_propulsion GTest::gtest_main)
        gtest_discover_tests(test_${suite})
    endforeach()
endif()
