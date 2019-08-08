#!/bin/bash

lcov --capture --directory . --output-file coverage_full.info > lcov_capture.txt
lcov --remove coverage_full.info '*/thirdParty/*' '*/tests/*' '*/include/ITK*' '*v1*' '*/ITKFactoryRegistration/*' '*/ITKIOFactoryRegistration/*' '*/usr/include/*' -o coverage.info > lcov_remove.txt
genhtml coverage.info --output-directory out
open out/index.html
