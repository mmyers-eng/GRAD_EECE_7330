cmake_minimum_required(VERSION 3.15)
project(DistributedNodeTests C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(./googletest)

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../app)
enable_testing()

add_executable(unit_tests
    ./TestCircBuffer.cpp
)

target_include_directories(unit_tests PUBLIC
  ${APP_DIR})
target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

# 5. Discover tests so CTest can run them automatically
include(GoogleTest)
gtest_discover_tests(unit_tests)
