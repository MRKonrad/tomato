###############
### TESTING ###
###############

if(BUILD_TESTING)

    find_package(gtest REQUIRED)

    enable_testing()

    # test files
    file(GLOB_RECURSE TESTS_FILES ${PROJECT_SOURCE_DIR}/tests/*.c* ${PROJECT_SOURCE_DIR}/tests/*.h* ${PROJECT_SOURCE_DIR}/tests/*.t*)

    # Test executable
    add_executable(TomatoTests ${TESTS_FILES})

    # link
    target_link_libraries(TomatoTests gtest::gtest)
    target_link_libraries(TomatoTests TomatoLib)

    # for the app tests
    target_include_directories(TomatoTests PUBLIC "${PROJECT_SOURCE_DIR}/app")
    add_definitions(-DTOMATOLIB_COMPILED)

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

    add_test(NAME TomatoTests COMMAND TomatoTests "--gtest_output=xml")

    # copy data and libs for runtime
    file(COPY testData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    if(WIN32)
      # copy Tomatolib for runtime
      # message(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/TomatoLib.dll)
      #  add_custom_command(
      #        TARGET TomatoTests
      #        POST_BUILD
      #      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/TomatoLib.dll ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
        file(COPY testData DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})        
    endif()

    install(TARGETS TomatoTests DESTINATION tests)

    # copy data for install time
    install(DIRECTORY ${CMAKE_BINARY_DIR}/tests/testData DESTINATION tests)

endif(BUILD_TESTING)
