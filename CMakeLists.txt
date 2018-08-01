##################
### OxShmolli2 ###
##################

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
file (GLOB_RECURSE APP_FILES app/*.c* app/*.h* app/*.t*)
file (GLOB_RECURSE LIB_FILES lib/*.c* lib/*.h* lib/*.t*)
set (LIB_FILES ${LIB_FILES} "${PROJECT_BINARY_DIR}/CmakeConfigForOxShmolli2.h")

#message("APP_FILES:" ${APP_FILES})
#message("LIB_FILES:" ${LIB_FILES})

include_directories (lib)

add_library (OxShmolli2Lib ${APP_FILES} ${LIB_FILES})
add_executable (OxShmolli2Exe ${APP_FILES} ${LIB_FILES})

############
### YAML ###
############

set (BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
add_subdirectory (thirdParty/libyaml)
include_directories (${yaml_BINARY_DIR}/include ${yaml_SOURCE_DIR}/include) # for windows
target_link_libraries (OxShmolli2Exe PUBLIC yaml)

###############
### Fitting ###
###############

set (BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
set (BUILD_CORE_GEOMETRY OFF CACHE INTERNAL "" FORCE)
set (BUILD_CORE_IMAGING OFF CACHE INTERNAL "" FORCE)
set (BUILD_CORE_NUMERICS ON CACHE INTERNAL "" FORCE)
set (BUILD_CORE_SERIALISATION OFF CACHE INTERNAL "" FORCE)
set (BUILD_CORE_UTILITIES OFF CACHE INTERNAL "" FORCE)
set (VXL_USE_GEOTIFF OFF CACHE INTERNAL "" FORCE)
set (VXL_USE_DCMTK OFF CACHE INTERNAL "" FORCE)
set (VXL_USE_LFS OFF CACHE INTERNAL "" FORCE)

add_subdirectory (thirdParty/vxl)

include_directories (${vxlcore_SOURCE_DIR}) # includes vnl
include_directories (${vxlcore_BINARY_DIR}) # includes vnl

include_directories (${vcl_SOURCE_DIR})
include_directories (${vcl_BINARY_DIR})

include_directories (${vgl_SOURCE_DIR})

if(WIN32)
    set (MYCONFIG $ENV{CONFIGURATION})
    set (MY_VXL_LIBRARY_PATH ${VXL_LIBRARY_PATH}/${MYCONFIG})
else(WIN32)
    set (MY_VXL_LIBRARY_PATH ${VXL_LIBRARY_PATH})
endif(WIN32)

link_directories (${MY_VXL_LIBRARY_PATH})
link_libraries (netlib v3p_netlib vcl vnl vnl_algo )

target_link_libraries (OxShmolli2Lib PUBLIC ${VNL_LIBRARIES})
target_link_libraries (OxShmolli2Exe PUBLIC ${VNL_LIBRARIES})
add_dependencies (OxShmolli2Lib vnl vnl_algo)
add_dependencies (OxShmolli2Exe vnl vnl_algo)

# install
set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "" FORCE)
install (TARGETS OxShmolli2Lib DESTINATION ${CMAKE_INSTALL_PREFIX}/install/lib)
install (FILES ${LIB_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/install/include/OxShmolli)
install (TARGETS OxShmolli2Exe DESTINATION ${CMAKE_INSTALL_PREFIX}/install/bin)


###############
### TESTING ###
###############

add_subdirectory (tests)

#############################
### DISPLAY ALL VARIABLES ###
#############################

get_cmake_property (_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message (STATUS "${_variableName}=${${_variableName}}")
endforeach()





