#!/usr/bin/env bash
git submodule init
git submodule update

#ARM cmsis setup for system headers.
cd source/vendor
git clone https://github.com/arm-software/cmsis_5 --depth=1


#gtest setup for unit tests.
cd -
cd source/tests
GTESTNAME="gtest.tar.gz"
#get gtest 1.18 and decompress the folder.
curl -L -o "${GTESTNAME}" "https://github.com/google/googletest/releases/download/v1.18.0/googletest-1.18.0.tar.gz"
tar -xzf "${GTESTNAME}"
rm -rf "${GTESTNAME}"
#rename the googletest folder
mv "googletest-1.18.0" "googletest"
