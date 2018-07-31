###############
### TESTING ###
###############

include(gtest.cmake)

enable_testing ()

# Threads are needed for testing
find_package (Threads REQUIRED)
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})

# Test executable
file(GLOB_RECURSE TESTS_FILES ${PROJECT_SOURCE_DIR}/tests/*.c* ${PROJECT_SOURCE_DIR}/tests/*.h* ${PROJECT_SOURCE_DIR}/tests/*.t*)
message("TESTS_FILES:" ${TESTS_FILES})

add_executable(OxShmolli2Tests ${TESTS_FILES})
target_link_libraries (OxShmolli2Tests PUBLIC gtest)
target_link_libraries (OxShmolli2Tests PUBLIC yaml)
target_link_libraries (OxShmolli2Tests ${VNL_LIBRARIES})

add_test(NAME ctest_tests COMMAND OxShmolli2Tests)
file(COPY testData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

########################
### TESTING COVERAGE ###
########################

# Compiler flags for coveralls, only in linux
if (UNIX AND NOT APPLE)
    set (CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
endif(UNIX AND NOT APPLE)

# Coveralls
SET(COVERAGE OFF CACHE BOOL "Coverage")
if (COVERAGE)
    target_compile_options(OxShmolli2Tests PRIVATE --coverage)
    target_link_libraries(OxShmolli2Tests PRIVATE --coverage)
endif()