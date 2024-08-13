cmake_minimum_required(VERSION 3.10)
project(dsa LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use FetchContent to download and build Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "main"
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Create an interface library for the header-only library
add_library(dsa INTERFACE)
target_include_directories(dsa INTERFACE ${CMAKE_SOURCE_DIR}/include/dsa)

# Add test sources
file(GLOB TEST_SOURCES "tests/*.cpp")

# Create the test executable
add_executable(runTests ${TEST_SOURCES})

# Link the test executable with Google Test libraries
target_link_libraries(runTests PRIVATE gtest gtest_main dsa)

# Register the tests
add_test(NAME runTests COMMAND runTests)

set_target_properties(dsa PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build
)
