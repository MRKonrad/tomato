if(USE_LMFIT)

    include_directories ("thirdParty/lmfit/include")
    link_directories(${CMAKE_SOURCE_DIR}/thirdParty/lmfit/lib)
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} lmfit)

endif(USE_LMFIT)