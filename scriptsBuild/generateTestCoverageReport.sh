#!/bin/bash

eval lcov --capture --directory . --output-file coverage_full.info
eval lcov --remove coverage_full.info '*/thirdParty/*' '*/tests/*' '*/include/ITK*' '*v1*' '*/ITKIOFactoryRegistration/*' '*/usr/include/*' -o coverage.info
eval genhtml coverage.info --output-directory out
eval open out/index.html
