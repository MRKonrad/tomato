# from clang --version
PATH="/Users/kwerys/Downloads/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin:$PATH"
DATE_WITH_TIME=`date "+%Y%m%d-%H%M%S"`

mkdir coverage
mkdir reports
cd coverage
conan install .. user/testing -s build_type=Debug -o use_yaml=True
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fprofile-instr-generate -fcoverage-mapping" -DCMAKE_C_FLAGS="-fprofile-instr-generate -fcoverage-mapping"
cmake --build .
cd tests
./TomatoTests > ../../reports/testReport_$DATE_WITH_TIME.log
llvm-profdata merge -sparse default.profraw -o default.profdata
llvm-cov report ./TomatoTests -instr-profile=default.profdata ../../lib/* ../../../tomato_private/lib/*
llvm-cov report ./TomatoTests -instr-profile=default.profdata ../../lib/* ../../../tomato_private/lib/* > ../../reports/coverageReport_$DATE_WITH_TIME.log