#!/bin/bash

# works on macos

cd ../..
mkdir tomato_test_coverage
cd tomato_test_coverage
rm -rf *
cmake ../tomato \
    -DCMAKE_CXX_FLAGS="--coverage" \
    -DCMAKE_INSTALL_PREFIX=install \
    -DCMAKE_CXX_STANDARD=98
make -j4
cd tests
./TomatoTests

cd ..
pwd
cp ../tomato/scriptsBuild/generateTestCoverageReport.sh generateTestCoverageReport.sh
ls
sh generateTestCoverageReport.sh
