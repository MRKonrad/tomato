######################################
### OxShmolli2 ###
######################################

cmake_minimum_required (VERSION 2.8)

project (OxShmolli2)

# The version number.
set (OxShmolli2_VERSION_MAJOR 0)
set (OxShmolli2_VERSION_MINOR 1)

# Compiler flags
set (CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/CmakeConfigForOxShmolli2.h.in" "${PROJECT_BINARY_DIR}/CmakeConfigForOxShmolli2.h")
include_directories ("${PROJECT_BINARY_DIR}")

####################
### MAIN PROGRAM ###
####################

# list of source files
file(GLOB_RECURSE APP_FILES app/*.c* app/*.h* app/*.t*)
file(GLOB_RECURSE LIB_FILES lib/*.c* lib/*.h* lib/*.t*)
set(LIB_FILES ${LIB_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForOxShmolli2.h")

#message("APP_FILES:" ${APP_FILES})
#message("LIB_FILES:" ${LIB_FILES})

include_directories(lib)

add_executable(OxShmolli2Exe ${APP_FILES} ${LIB_FILES})
add_library(OxShmolli2Lib ${APP_FILES} ${LIB_FILES})

###############
### TESTING ###
###############

add_subdirectory(tests)

