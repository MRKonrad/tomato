###############
### TESTING ###
###############

if(Tomato_BUILD_TESTING)

    include(../cmake/gtest.cmake)

    enable_testing()

    # Threads are needed for testing
    find_package(Threads REQUIRED)
    target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})

    # Test executable
    file(GLOB_RECURSE TESTS_FILES ${PROJECT_SOURCE_DIR}/tests/*.c* ${PROJECT_SOURCE_DIR}/tests/*.h* ${PROJECT_SOURCE_DIR}/tests/*.t*)

    add_executable(TomatoTests ${TESTS_FILES})
    target_link_libraries(TomatoTests gtest)
    target_link_libraries(TomatoTests TomatoLib)

    add_test(NAME ctest_tests COMMAND TomatoTests)

    file(COPY testData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    install(TARGETS TomatoTests DESTINATION tests)
    install(DIRECTORY ${CMAKE_BINARY_DIR}/tests/testData DESTINATION tests)

endif(Tomato_BUILD_TESTING)
