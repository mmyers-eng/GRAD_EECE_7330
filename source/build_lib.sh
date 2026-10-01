#!/usr/bin/env bash
#This script will produce a libtensorflow-microlite.a library file which
#gets linked into the project.
rm -rf ./tflite-micro/gen/cortex_m_generic_cortex-m4_default_gcc/lib/libtensorflow-microlite.a
rm -rf ./lib/microlite/libtensorflow-microlite.a
cd tflite-micro

# Important: tfml uses -mfloat-abi=soft by default
# your application MUST match it or prepare to deal with
# very strange problems with calculations or crashes.
# - j2 is used since the TFLM compilation process can use
# a massive amount of resources and crash the host machine.
make -j2 -f tensorflow/lite/micro/tools/make/Makefile \
  TARGET=cortex_m_generic \
  TARGET_ARCH=cortex-m4 \
  microlite
cd -
mkdir -p lib/microlite

cp tflite-micro/gen/cortex_m_generic_cortex-m4_default_gcc/lib/libtensorflow-microlite.a lib/microlite/libtensorflow-microlite.a
