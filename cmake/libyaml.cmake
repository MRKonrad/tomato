if(USE_YAML)

    set (BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
    set (BUILD_SHARED_LIBS ON CACHE INTERNAL "" FORCE)
    add_subdirectory (thirdParty/libyaml)
    include_directories (${yaml_BINARY_DIR}/include ${yaml_SOURCE_DIR}/include) # for windows
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} yaml)

endif(USE_YAML)