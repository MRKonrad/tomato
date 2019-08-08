if(USE_LMFIT)
    add_subdirectory(thirdParty/lmfit)
    include_directories (${lmfit_BINARY_DIR}/lib ${lmfit_SOURCE_DIR}/lib) # for windows
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} lmfit)
endif(USE_LMFIT)