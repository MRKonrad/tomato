##################
### OxShmolli2 ###
##################

cmake_minimum_required (VERSION 2.8)

project (OxShmolli2)

# The version number.
set (OxShmolli2_VERSION_MAJOR 0)
set (OxShmolli2_VERSION_MINOR 1)

# Compiler flags
set (CMAKE_CXX_STANDARD 98 CACHE STRING "")
set (CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_STANDARD EQUAL 98)
    set (CXX_STANDARD_98 ON) # flag used in the code
endif()

# list of source files
file (GLOB_RECURSE APP_FILES app/*.c* app/*.h* app/*.t*)
file (GLOB_RECURSE LIB_FILES lib/*.c* lib/*.h* lib/*.t*)

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

## VNL TODO: code behaviour with VNL off
#set(USE_VNL ON CACHE BOOL "Do you want to use VNL numerics library?")
#include(cmake/vnl.cmake)

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

###########
### ITK ###
###########

set(USE_ITK ON CACHE BOOL "Do you want to use ITK?")
if(USE_ITK)
    set(USE_ITK OFF)

    find_package(ITK) # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (NOT ITK_FOUND)
        find_package(ITK HINTS ../ITK_install)
    endif()
    MESSAGE("ITK version:" ${ITK_VERSION})
    if (ITK_FOUND)
        set(USE_ITK ON)
        set(USE_VNL ON)
        include(${ITK_USE_FILE})
        set(OxSmolli2_LIBS_TO_LINK ${OxSmolli2_LIBS_TO_LINK} ${ITK_LIBRARIES})
    endif()
endif(USE_ITK)

###########
### VTK ###
###########

set(USE_VTK ON CACHE BOOL "Do you want to use VTK?")
list (FIND ITK_LIBRARIES "ITKVtkGlue" _index)
# use vtk only if ITK is present
if(USE_VTK)
    set(USE_VTK OFF)
    if(_index GREATER -1)
        find_package(VTK)
        MESSAGE("VTK version:" ${VTK_VERSION})
        if (VTK_FOUND)
            set(USE_VTK ON)
            include(${VTK_USE_FILE})
            set(OxSmolli2_LIBS_TO_LINK ${OxSmolli2_LIBS_TO_LINK} ${VTK_LIBRARIES})
        endif()
    endif()
endif(USE_VTK)

####################
### MAIN PROGRAM ###
####################

MESSAGE(${CXX_STANDARD_98})
MESSAGE(${USE_ITK})
MESSAGE(${USE_VTK})
MESSAGE(${USE_VNL})
MESSAGE(${USE_PRIVATE_NR2})
MESSAGE(${USE_NR2})
MESSAGE(${USE_NR3})

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/CmakeConfigForOxShmolli2.h.in" "${PROJECT_BINARY_DIR}/CmakeConfigForOxShmolli2.h")
set (LIB_FILES ${LIB_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForOxShmolli2.h")

include_directories (app)
include_directories (lib)
include_directories ("${PROJECT_BINARY_DIR}") # for the configuration header

add_library (OxShmolli2Lib ${APP_FILES} ${LIB_FILES})
add_executable (OxShmolli2Exe ${APP_FILES} ${LIB_FILES})

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





