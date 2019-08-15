#!/bin/bash

git submodule update --init --recursive

cd ../..
mkdir tomato_test_coverage
cd tomato_test_coverage
rm -rf *
cmake ../tomato \
    -DCMAKE_CXX_FLAGS="--coverage" \
    -DCMAKE_INSTALL_PREFIX=install \
    -DCMAKE_CXX_STANDARD=98 \
    -DITK_DIR_HINTS="../ITK_install"

cmake --build . --config DEBUG --parallel 6
# cmake --build . --config DEBUG --target install

cd tests
./TomatoTests

cd ..
pwd
cp ../tomato/scriptsBuild/generateTestCoverageReport.sh generateTestCoverageReport.sh
ls
sh generateTestCoverageReport.sh
