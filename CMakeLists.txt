##############
### Tomato ###
##############

cmake_minimum_required (VERSION 2.8)

project (Tomato)

# The version number.
set (Tomato_VERSION_MAJOR 0)
set (Tomato_VERSION_MINOR 3)

# Compiler flags
set (CMAKE_CXX_STANDARD 98 CACHE STRING "")
set (CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_STANDARD EQUAL 98)
    set (CXX_STANDARD_98 ON) # flag used in the code
endif()
set(CMAKE_MACOSX_RPATH 1) # to address MACOSX_RPATH warning

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=undefined")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=undefined")

# list of source files
file (GLOB_RECURSE APP_FILES app/*.c* app/*.h* app/*.t*)
file (GLOB_RECURSE LIB_FILES lib/*.c* lib/*.h* lib/*.t*)
file (GLOB_RECURSE API_FILES lib/*.h lib/KWUtil.hxx)

###############
### PRIVATE ###
###############

# unfortunatelly I cannot publish numerical recipes code according to numerical recipes licence
set(PRIVATE_NR2_DIR ../tomato_private CACHE PATH "")
set(USE_PRIVATE_NR2 ON CACHE BOOL "")
if (USE_PRIVATE_NR2)
    add_subdirectory(${PRIVATE_NR2_DIR} ${CMAKE_BINARY_DIR}/private)
    include_directories(${PRIVATE_NR2_DIR}/lib)
endif()

############
### YAML ###
############

set(USE_YAML ON CACHE BOOL "Do you want to use Numerical Recipes 2 code?")
include(cmake/libyaml.cmake)

###############
### Fitting ###
###############

## VNL TODO: code behaviour with VNL off
#set(USE_VNL ON CACHE BOOL "Do you want to use VNL numerics library?")
#include(cmake/vnl.cmake)

#target_link_libraries (TomatoLib PUBLIC ${VNL_LIBRARIES})
#target_link_libraries (TomatoExe PUBLIC ${VNL_LIBRARIES})
#add_dependencies (TomatoLib vnl vnl_algo)
#add_dependencies (TomatoExe vnl vnl_algo)

# Numerical Recipes v2
set(USE_NR2 OFF CACHE BOOL "Do you want to use Numerical Recipes 2 code?")
include(cmake/nr2.cmake)

# Numerical Recipes v3
set(USE_NR3 OFF CACHE BOOL "Do you want to use Numerical Recipes 3 code?")
include(cmake/nr3.cmake)

###########
### ITK ###
###########

set(USE_ITK ON CACHE BOOL "Do you want to use ITK?")
if(USE_ITK)
    set(USE_ITK OFF)
    set(USE_VNL OFF)
    find_package(ITK) # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (NOT ITK_FOUND)
        find_package(ITK HINTS ../ITK_install)
    endif()
    MESSAGE("ITK version:" ${ITK_VERSION})
    if (ITK_FOUND)
        set(USE_ITK ON)
        set(USE_VNL ON)
        include(${ITK_USE_FILE})
        set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} ${ITK_LIBRARIES})
    endif()
else()
    set(USE_VNL OFF)
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
            set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} ${VTK_LIBRARIES})
        endif()
    endif()
endif(USE_VTK)

####################
### MAIN PROGRAM ###
####################

set(BUILD_APP OFF)
if(USE_ITK)
    if(USE_YAML)
        set(BUILD_APP ON)
    endif(USE_YAML)
endif(USE_ITK)            

MESSAGE("CXX_STANDARD_98:   " ${CXX_STANDARD_98})
MESSAGE("USE_ITK:           " ${USE_ITK})
MESSAGE("USE_VTK:           " ${USE_VTK})
MESSAGE("USE_VNL:           " ${USE_VNL})
MESSAGE("USE_PRIVATE_NR2:   " ${USE_PRIVATE_NR2})
MESSAGE("USE_NR2:           " ${USE_NR2})
MESSAGE("USE_NR3:           " ${USE_NR3})
MESSAGE("USE_YAML:          " ${USE_YAML})
MESSAGE("BUILD_SHARED_LIBS: " ${BUILD_SHARED_LIBS})
MESSAGE("BUILD_APP:         " ${BUILD_APP})

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/CmakeConfigForTomato.h.in" "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")
set (LIB_FILES ${LIB_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")
set (API_FILES ${API_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")

if(BUILD_APP)
    include_directories (app)
endif(BUILD_APP)
include_directories (lib)
include_directories ("${PROJECT_BINARY_DIR}") # for the configuration header

# targets
if(BUILD_APP)
    add_library (TomatoLib ${APP_FILES} ${LIB_FILES})
    add_executable (TomatoExe ${APP_FILES} ${LIB_FILES})
    set_target_properties(TomatoExe PROPERTIES COMPILE_FLAGS -DTomatoLib_EXPORTS) # to see hxx content
else(BUILD_APP)
    add_library (TomatoLib ${LIB_FILES})
endif(BUILD_APP)

# generating TomatoLib_export.h
include(GenerateExportHeader)
generate_export_header(TomatoLib)
set(API_FILES ${API_FILES} ${PROJECT_BINARY_DIR}/tomatolib_export.h) # to make tomatolib_export.h available for export

###############
### Linking ###
###############

target_link_libraries (TomatoLib PUBLIC ${TOMATO_LIBS_TO_LINK})
set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} TomatoLib)
if(BUILD_APP)
    target_link_libraries (TomatoExe PUBLIC ${TOMATO_LIBS_TO_LINK})
endif(BUILD_APP)


##################
### Installing ###
##################

#set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../tomato_install) # TODO: Should be configure from cmake command line

install (TARGETS TomatoLib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install (FILES ${API_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
if(BUILD_APP)
    install (TARGETS TomatoExe DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif(BUILD_APP)

###############
### TESTING ###
###############

set(Tomato_BUILD_TESTING ON CACHE BOOL "Do you want to build Tomato tests?")
if(USE_YAML)
    add_subdirectory (tests)
endif(USE_YAML)

#############################
### DISPLAY ALL VARIABLES ###
#############################

#get_cmake_property (_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message (STATUS "${_variableName}=${${_variableName}}")
#endforeach()
