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
# additionally for linux:
#       -DCMAKE_POSITION_INDEPENDENT_CODE=ON
# see .travis.yml and appveyor.yml for examples
#
# Konrad Werys

cmake_minimum_required(VERSION 2.8)

project(Tomato)

# The version number
set(Tomato_VERSION_MAJOR 0)
set(Tomato_VERSION_MINOR 5)

# Compiler flags
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 98 CACHE STRING "")
set(CXX_STANDARD_98 OFF)
if(CMAKE_CXX_STANDARD EQUAL 98)
    set(CXX_STANDARD_98 ON) # flag used in the code
endif()

# needed for conan
set(CMAKE_MODULE_PATH  ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

#####################
### BUILD OPTIONS ###
#####################

set(USE_ITK OFF CACHE BOOL "Do you want to use ITK?")
set(USE_VNL ON CACHE BOOL "Do you want to use VNL?")
set(USE_YAML ON CACHE BOOL "Do you want to use yaml?")
set(USE_LMFIT ON CACHE BOOL "Do you want to use LMFIT?")
set(USE_TOMATOFIT ON CACHE BOOL "Do you want to use TOMATOFIT?")
set(DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "Do you want to download the dependencies?")

##################
### LIST FILES ###
##################

# list of source files TODO: do not use glob, as it is not recommended https://cmake.org/cmake/help/latest/command/file.html#filesystem
file(GLOB_RECURSE APP_FILES app/*.c* app/*.h* app/*.t*)
file(GLOB_RECURSE LIB_FILES lib/*.c* lib/*.h* lib/*.t*)
file(GLOB_RECURSE API_FILES lib/*.h app/*.h lib/KWUtil.hxx)

#############################
### DOWNLOAD DEPENDENCIES ###
#############################

include(cmake/downloadItk.cmake)
include(cmake/downloadLmfit.cmake)

###########
### ITK ###
###########

if(USE_ITK)
    set(USE_ITK OFF)
    find_package(ITK HINTS ${ITK_DIR_HINTS})
    MESSAGE("ITK version:" ${ITK_VERSION})
    if(ITK_FOUND)
        set(USE_ITK ON)
        set(USE_VNL ON)
        include(${ITK_USE_FILE})
        set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} ${ITK_LIBRARIES})
    endif()
endif(USE_ITK)

###########
### VNL ###
###########

if (USE_VNL)
    find_package(vxl REQUIRED)
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} vxl::vxl)
    include_directories(${vxl_INCLUDE_DIRS}/vxl/core)
    include_directories(${vxl_INCLUDE_DIRS}/vxl/vcl)
endif()

############
### YAML ###
############

if (USE_YAML)
    find_package(libyaml REQUIRED)
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} libyaml::libyaml)
endif()

#############
### LMFIT ###
#############

if (USE_LMFIT)
    find_package(lmfit REQUIRED)
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} lmfit::lmfit)
endif()

###############
### PRIVATE ###
###############

# unfortunately I cannot publish numerical recipes code according to numerical recipes licence
set(PRIVATE_NR2_DIR ${PROJECT_SOURCE_DIR}/../tomato_private CACHE PATH "")
set(USE_PRIVATE_NR2 OFF CACHE BOOL "")
if(USE_PRIVATE_NR2 AND EXISTS ${PRIVATE_NR2_DIR})
    add_subdirectory(${PRIVATE_NR2_DIR} ${CMAKE_BINARY_DIR}/private)
    include_directories(${PRIVATE_NR2_DIR}/lib)
else()
    set(USE_PRIVATE_NR2 OFF)
endif()

#################
### TOMATOFIT ###
#################

include(cmake/tomatofit.cmake)

####################
### MAIN PROGRAM ###
####################

set(BUILD_TESTING ON CACHE BOOL "Do you want to build Tomato tests?")
set(BUILD_APP ON CACHE BOOL "Do you want to build Tomato app?")
if (BUILD_APP)
    if(NOT USE_ITK OR NOT USE_YAML)
        set(BUILD_APP OFF)
    endif()
endif()

MESSAGE("CXX_STANDARD_98:       " ${CXX_STANDARD_98})
MESSAGE("DOWNLOAD_DEPENDENCIES: " ${DOWNLOAD_DEPENDENCIES})
MESSAGE("USE_ITK:               " ${USE_ITK})
MESSAGE("USE_VNL:               " ${USE_VNL})
MESSAGE("USE_PRIVATE_NR2:       " ${USE_PRIVATE_NR2})
MESSAGE("USE_LMFIT:             " ${USE_LMFIT})
MESSAGE("USE_TOMATOFIT:         " ${USE_TOMATOFIT})
MESSAGE("USE_YAML:              " ${USE_YAML})
MESSAGE("BUILD_APP:             " ${BUILD_APP})
MESSAGE("BUILD_TESTING:         " ${BUILD_TESTING})
MESSAGE("CMAKE_INSTALL_PREFIX:  " ${CMAKE_INSTALL_PREFIX})

# configure a header file to pass some of the CMake settings to the source code
configure_file("${PROJECT_SOURCE_DIR}/cmake/CmakeConfigForTomato.h.in" "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")
set(LIB_FILES ${LIB_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")
set(API_FILES ${API_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForTomato.h")

#################
### TOMATOLIB ###
#################

# include
include_directories(lib)
include_directories("${PROJECT_BINARY_DIR}") # for the configuration header

# target
add_library(TomatoLib ${LIB_FILES})
set_target_properties(TomatoLib PROPERTIES LINKER_LANGUAGE CXX)

#link
target_link_libraries(TomatoLib ${TOMATO_LIBS_TO_LINK})
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
    include_directories(app)

    # target
    add_executable(TomatoExe ${APP_FILES} ${LIB_FILES})

    # link
    target_link_libraries(TomatoExe ${TOMATO_LIBS_TO_LINK})

    # RPATH preparation for the executables
    set_target_properties(TomatoExe PROPERTIES INSTALL_RPATH
            @executable_path
            @executable_path/lib
            @executable_path/../lib
            @loader_path
            @loader_path/lib
            @loader_path/../lib
            $ORIGIN
            $ORIGIN/lib
            $ORIGIN/../lib/)
endif()

##################
### Installing ###
##################

install(TARGETS TomatoLib
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
install(FILES ${API_FILES} DESTINATION include) # headers
if(BUILD_APP)
    install(TARGETS TomatoExe DESTINATION bin)
endif()

###############
### TESTING ###
###############

if(BUILD_TESTING AND USE_YAML)
    add_subdirectory(tests)
    install(TARGETS TomatoLib DESTINATION tests)
endif()