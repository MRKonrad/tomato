
if(USE_VNL)
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

    set (TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} ${VNL_LIBRARIES})
endif(USE_VNL)