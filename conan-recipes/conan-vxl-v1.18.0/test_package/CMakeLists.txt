cmake_minimum_required(VERSION 2.8.12)
project(PackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CONAN_INCLUDE_DIRS_VXL}/vxl/core)
include_directories(${CONAN_INCLUDE_DIRS_VXL}/vxl/vcl)
add_executable(${PROJECT_NAME} example.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

if(APPLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND
        ${CMAKE_INSTALL_NAME_TOOL}
        -add_rpath
        "@loader_path/."
        $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Inject rpath"
    )
endif()

# CTest is a testing tool that can be used to test your project.
# enable_testing()
# add_test(NAME example
#          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#          COMMAND example)
