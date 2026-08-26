#!/usr/bin/env bash
mkdir build
cd ./build
cmake -DCMAKE_TOOLCHAIN_FILE=./stm32-ml.cmake ..
make
#qemu-system-arm -M netduinoplus2 -cpu cortex-m4 -nographic -kernel qemu_app
