#!/usr/bin/env bash
git submodule init
git submodule update

cd source/vendor
git clone https://github.com/arm-software/cmsis_5 --depth=1
