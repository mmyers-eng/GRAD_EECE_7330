#!/usr/bin/env bash
# Note: this script must be launched within the docker environment see README.md for details
qemu-system-arm -M netduinoplus2 \
                -cpu cortex-m4 \
                -nographic \
                -kernel ./build/qemu_app.elf\
                -serial tcp:127.0.0.1:4444,server
