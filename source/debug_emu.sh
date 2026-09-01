#!/usr/bin/env bash
# Note: this script must be launched within the docker environment see README.md for details
qemu-system-arm -M netduinoplus2 -cpu cortex-m4 -nographic -kernel ./build/qemu_app -gdb tcp::1234 -S
