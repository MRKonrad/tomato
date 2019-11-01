##############
### Tomato ###
##############
# you can build tomato with command:
# cmake .
#       -DCMAKE_BUILD_TYPE=RELEASE
#       -DCMAKE_INSTALL_PREFIX="install"
# additionally for osx:
#       -DCMAKE_MACOSX_RPATH=1
# additionally for windows:
#       -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON
# see .travis.yml and appveyor.yml for examples
#
# Konrad Werys

cmake_minimum_required (VERSION 2.8)

project (Tomato)

# The version number
set (Tomato_VERSION_MAJOR 0)
set (Tomato_VERSION_MINOR 5)

# Compiler flags
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 98 CACHE STRING "")
if (CMAKE_CXX_STANDARD EQUAL 98)
    set (CXX_STANDARD_98 ON) # flag used in the code
endif()

# RPATH preparation for the executables
set(CMAKE_INSTALL_RPATH
        @executable_path
        @executable_path/lib
        @executable_path/../lib
        @loader_path
        @loader_path/lib
        @loader_path/../lib
        $ORIGIN
        $ORIGIN/lib
        $ORIGIN/../lib/)

#############################
### DOWNLOAD DEPENDENCIES ###
#############################

# scripts to download pre-compiled dependencis, running only if DOWNLOAD_DEPENDENCIES is ON
set(DOWNLOAD_DEPENDENCIES ON CACHE BOOL "Do you want to download the dependencies?")
include(cmake/downloadItk.cmake)
include(cmake/downloadLmfit.cmake)

##################
### list files ###
##################

# list of source files TODO: do not use glob, as it is not recommended https://cmake.org/cmake/help/latest/command/file.html#filesystem
file (GLOB_RECURSE APP_FILES app/*.c* app/*.h* app/*.t*)
file (GLOB_RECURSE LIB_FILES lib/*.c* lib/*.h* lib/*.t*)
file (GLOB_RECURSE API_FILES lib/*.h lib/KWUtil.hxx)

###############
### PRIVATE ###
###############

# unfortunatelly I cannot publish numerical recipes code according to numerical recipes licence
set(PRIVATE_NR2_DIR ../tomato_private CACHE PATH "")
set(USE_PRIVATE_NR2 OFF CACHE BOOL "")
if (USE_PRIVATE_NR2)
    add_subdirectory(${PRIVATE_NR2_DIR} ${CMAKE_BINARY_DIR}/private)
    include_directories(${PRIVATE_NR2_DIR}/lib)
endif()

############
### YAML ###
############

set(USE_YAML ON CACHE BOOL "Do you want to use yaml?")
include(cmake/libyaml.cmake)

###############
### Fitting ###
###############

# lmfit currently not on windows
set(USE_LMFIT ON CACHE BOOL "Do you want to use LMFIT?")
include(cmake/lmfit.cmake)

###########
### ITK ###
###########

set(USE_ITK ON CACHE BOOL "Do you want to use ITK?")

if(USE_ITK)
    set(USE_ITK OFF)
    set(USE_VNL OFF)
    find_package(ITK HINTS ${ITK_DIR_HINTS})
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

####################
### MAIN PROGRAM ###
####################

set(Tomato_BUILD_TESTING ON CACHE BOOL "Do you want to build Tomato tests?")

set(BUILD_APP OFF)
if(USE_ITK AND  USE_YAML)
    set(BUILD_APP ON)
endif()

MESSAGE("CXX_STANDARD_98:      " ${CXX_STANDARD_98})
MESSAGE("DOWNLOAD_DEPENDENCIES:" ${DOWNLOAD_DEPENDENCIES})
MESSAGE("USE_ITK:              " ${USE_ITK})
MESSAGE("USE_VNL:              " ${USE_VNL})
MESSAGE("USE_PRIVATE_NR2:      " ${USE_PRIVATE_NR2})
MESSAGE("USE_LMFIT:            " ${USE_LMFIT})
MESSAGE("USE_YAML:             " ${USE_YAML})
MESSAGE("BUILD_APP:            " ${BUILD_APP})
MESSAGE("Tomato_BUILD_TESTING: " ${Tomato_BUILD_TESTING})
MESSAGE("BUILD_SHARED_LIBS:    " ${BUILD_SHARED_LIBS})
MESSAGE("CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/CmakeConfigForTomato.h.in" "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")
set (LIB_FILES ${LIB_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")
set (API_FILES ${API_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")

#################
### TOMATOLIB ###
#################

# include
include_directories (lib)
include_directories ("${PROJECT_BINARY_DIR}") # for the configuration header

# target
add_library (TomatoLib SHARED ${LIB_FILES})

#link
target_link_libraries (TomatoLib ${TOMATO_LIBS_TO_LINK})
set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} TomatoLib)

# generating TomatoLib_export.h
include(GenerateExportHeader)
generate_export_header(TomatoLib)
set(API_FILES ${API_FILES} ${PROJECT_SOURCE_DIR}/cmake/tomatolib_export.h) # to make tomatolib_export.h available in installation step

#################
### TOMATOAPP ###
#################

if(BUILD_APP)

    # include
    include_directories (app)

    # target
    add_executable (TomatoExe ${APP_FILES} ${LIB_FILES})

    # link
    target_link_libraries (TomatoExe ${TOMATO_LIBS_TO_LINK})

endif()

##################
### Installing ###
##################

install (TARGETS TomatoLib
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib)
install (FILES ${API_FILES} DESTINATION include) # headers
if(BUILD_APP)
    install (TARGETS TomatoExe DESTINATION bin)
endif()

###############
### TESTING ###
###############

if(Tomato_BUILD_TESTING AND USE_YAML)
    add_subdirectory (tests)
    install(TARGETS TomatoLib DESTINATION tests)
endif()

##############################
#### DISPLAY ALL VARIABLES ###
##############################
#
##get_cmake_property (_variableNames VARIABLES)
##list (SORT _variableNames)
##foreach (_variableName ${_variableNames})
##    message (STATUS "${_variableName}=${${_variableName}}")
##endforeach()
