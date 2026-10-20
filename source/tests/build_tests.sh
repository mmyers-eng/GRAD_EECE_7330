#!/usr/bin/env bash
# builds and runs all tests
# this file is intended to be run from the host system
# and only requires c++ 17 and GCC.
cmake -B build -S .
cmake --build build
cd build && ctest --output-on-failure
