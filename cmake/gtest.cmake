# based on http://david-grs.github.io/cpp-clang-travis-cmake-gtest-coveralls-appveyor/

set(GOOGLETEST_ROOT thirdParty/googletest/googletest CACHE STRING "Google Test source root")
# for version 1.4 of gtest:
if(EXISTS ${PROJECT_SOURCE_DIR}/thirdParty/googletest/src/gtest_main.cc)
    set(GOOGLETEST_ROOT thirdParty/googletest)
endif()

include_directories(SYSTEM
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
    )

set(GOOGLETEST_SOURCES
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
    )

foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(gtest STATIC ${GOOGLETEST_SOURCES})
