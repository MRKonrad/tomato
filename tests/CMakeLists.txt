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
    #message("TESTS_FILES:" ${TESTS_FILES})

    add_executable(TomatoTests ${TESTS_FILES})
    target_link_libraries(TomatoTests PUBLIC gtest)
    target_link_libraries(TomatoTests PUBLIC ${TOMATO_LIBS_TO_LINK})
    set_target_properties(TomatoTests PROPERTIES COMPILE_FLAGS -DTomatoLib_EXPORTS) # to see hxx content
    #add_dependencies(TomatoTests TomatoLib)

    add_test(NAME ctest_tests COMMAND TomatoTests)

    file(COPY testData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(GLOB_RECURSE TESTDATA ${PROJECT_SOURCE_DIR}/tests/testData/*.*)

    set(testDataDir ${CMAKE_BINARY_DIR}/tests/testData/)
    if(WIN32)
        set(testDataDir ${CMAKE_BINARY_DIR}/tests/${CMAKE_BUILD_TYPE}/testData/)
    endif(WIN32)

    install(TARGETS TomatoTests DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
    # install (FILES ${TESTDATA} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/testData/)
    install(DIRECTORY ${testDataDir} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/testData)

    ########################
    ### TESTING COVERAGE ###
    ########################

    # Compiler flags for coveralls, only in linux
    if (UNIX AND NOT APPLE)
        set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
    endif(UNIX AND NOT APPLE)

    # Coveralls
    SET(COVERAGE OFF CACHE BOOL "Coverage")
    if (COVERAGE)
        target_compile_options(TomatoTests PRIVATE --coverage)
        target_link_libraries(TomatoTests PRIVATE --coverage)
    endif()

endif(Tomato_BUILD_TESTING)