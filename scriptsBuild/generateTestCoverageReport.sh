#!/bin/bash

lcov --capture --directory . --output-file coverage_full.info
lcov --remove coverage_full.info '*/thirdParty/*' '*/tests/*' '*/include/ITK*' '*v1*' '*/ITKIOFactoryRegistration/*' '*/usr/include/*' -o coverage.info
genhtml coverage.info --output-directory out
open out/index.html
