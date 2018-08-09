##################
### OxShmolli2 ###
##################

cmake_minimum_required (VERSION 2.8)

project (OxShmolli2)

# The version number.
set (OxShmolli2_VERSION_MAJOR 0)
set (OxShmolli2_VERSION_MINOR 1)

# Compiler flags

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

#set (CMAKE_CXX_STANDARD 98)
#set (CMAKE_CXX_STANDARD_REQUIRED ON)
#
#if (CMAKE_CXX_STANDARD EQUAL 98)
#    set (CXX_STANDARD_98 ON)
#endif()

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/CmakeConfigForOxShmolli2.h.in" "${PROJECT_BINARY_DIR}/CmakeConfigForOxShmolli2.h")
include_directories ("${PROJECT_BINARY_DIR}")

# list of source files
file (GLOB_RECURSE APP_FILES app/*.c* app/*.h* app/*.t*)
file (GLOB_RECURSE LIB_FILES lib/*.c* lib/*.h* lib/*.t*)
set (LIB_FILES ${LIB_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForOxShmolli2.h")

###############
### PRIVATE ###
###############

# unfortunatelly I cannot publish numerical recipes code according to numerical recipes licence
set(PRIVATE_NR2_DIR ../OxShmolli2_private CACHE PATH "")
set(USE_PRIVATE_NR2 ON CACHE BOOL "")
if (USE_PRIVATE_NR2)
    add_subdirectory(${PRIVATE_NR2_DIR} ${CMAKE_BINARY_DIR}/private)
    include_directories(${PRIVATE_NR2_DIR}/lib)
endif()


####################
### MAIN PROGRAM ###
####################

include_directories (lib)

add_library (OxShmolli2Lib ${APP_FILES} ${LIB_FILES})
add_executable (OxShmolli2Exe ${APP_FILES} ${LIB_FILES})

############
### YAML ###
############

set (BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
add_subdirectory (thirdParty/libyaml)
include_directories (${yaml_BINARY_DIR}/include ${yaml_SOURCE_DIR}/include) # for windows
set(OxSmolli2_LIBS_TO_LINK ${OxSmolli2_LIBS_TO_LINK} yaml)
#target_link_libraries (OxShmolli2Exe PUBLIC yaml)

###############
### Fitting ###
###############

# VNL TODO: code behaviour with VNL off
set(USE_VNL ON CACHE BOOL "Do you want to use VNL numerics library?")
include(cmake/vnl.cmake)

#target_link_libraries (OxShmolli2Lib PUBLIC ${VNL_LIBRARIES})
#target_link_libraries (OxShmolli2Exe PUBLIC ${VNL_LIBRARIES})
#add_dependencies (OxShmolli2Lib vnl vnl_algo)
#add_dependencies (OxShmolli2Exe vnl vnl_algo)

# Numerical Recipes v2
set(USE_NR2 ON CACHE BOOL "Do you want to use Numerical Recipes 2 code?")
include(cmake/nr2.cmake)
#target_link_libraries (OxShmolli2Lib PUBLIC NR2)

# Numerical Recipes v3
set(USE_NR3 OFF CACHE BOOL "Do you want to use Numerical Recipes 3 code?")
include(cmake/nr3.cmake)

###############
### Linking ###
###############
target_link_libraries (OxShmolli2Lib PUBLIC ${OxSmolli2_LIBS_TO_LINK})
set(OxSmolli2_LIBS_TO_LINK ${OxSmolli2_LIBS_TO_LINK} OxShmolli2Lib)
target_link_libraries (OxShmolli2Exe PUBLIC ${OxSmolli2_LIBS_TO_LINK})

##################
### Installing ###
##################

set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "" FORCE)
install (TARGETS OxShmolli2Lib DESTINATION ${CMAKE_INSTALL_PREFIX}/install/lib)
install (FILES ${LIB_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/install/include/OxShmolli)
install (TARGETS OxShmolli2Exe DESTINATION ${CMAKE_INSTALL_PREFIX}/install/bin)

###############
### TESTING ###
###############

set(OxShmolli2_BUILD_TESTING ON CACHE BOOL "Do you want to build OxShmolli2 tests?")
add_subdirectory (tests)

#############################
### DISPLAY ALL VARIABLES ###
#############################

#get_cmake_property (_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message (STATUS "${_variableName}=${${_variableName}}")
#endforeach()





