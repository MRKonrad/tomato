cmake_minimum_required(VERSION 2.8.12)
project(PackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} example.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

# KW: I do not understand why it has to be linked separately, it is in CONAN_LIBS variable
target_link_libraries(${PROJECT_NAME} v3p_netlib)
