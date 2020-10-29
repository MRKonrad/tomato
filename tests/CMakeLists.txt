###############
### TESTING ###
###############

if(BUILD_TESTING)
    find_package(gtest REQUIRED)
    enable_testing()

    # Test executable
    file(GLOB_RECURSE TESTS_FILES ${PROJECT_SOURCE_DIR}/tests/*.c* ${PROJECT_SOURCE_DIR}/tests/*.h* ${PROJECT_SOURCE_DIR}/tests/*.t*)

    add_executable(TomatoTests ${TESTS_FILES})
    target_link_libraries(TomatoTests gtest::gtest)
    target_link_libraries(TomatoTests TomatoLib)

    # RPATH preparation for the executables
    set_target_properties(TomatoTests PROPERTIES INSTALL_RPATH
            @executable_path
            @executable_path/lib
            @executable_path/../lib
            @loader_path
            @loader_path/lib
            @loader_path/../lib
            $ORIGIN
            $ORIGIN/lib
            $ORIGIN/../lib/)

    add_test(NAME ctest_tests COMMAND TomatoTests)

    file(COPY testData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    install(TARGETS TomatoTests DESTINATION tests)
    install(DIRECTORY ${CMAKE_BINARY_DIR}/tests/testData DESTINATION tests)

endif(BUILD_TESTING)
