#!/usr/bin/env bash
#This script will produce a libtensorflow-microlite.a library file which
#gets linked into the project.
cd tflite-micro
make -j -f tensorflow/lite/micro/tools/make/Makefile \
  TARGET=cortex_m_generic \
  TARGET_ARCH=cortex-m4 \
  FLOAT_ABI=hard \
  microlite
cd -
mkdir -p lib/microlite
cp tflite-micro/gen/cortex_m_generic_cortex-m4_default_gcc/lib/libtensorflow-microlite.a lib/microlite/libtensorflow-microlite.a
