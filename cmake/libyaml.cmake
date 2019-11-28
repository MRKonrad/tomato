if(USE_YAML)

    set (BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
    add_subdirectory (thirdParty/libyaml)
    include_directories (${yaml_BINARY_DIR}/include ${yaml_SOURCE_DIR}/include) # for windows
    set_property(TARGET yaml PROPERTY POSITION_INDEPENDENT_CODE ON)
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} yaml)

endif(USE_YAML)