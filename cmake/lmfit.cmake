if(USE_LMFIT)

    # this way brings unexplained cmake errors
    #    add_subdirectory(thirdParty/lmfit)
    #    include_directories (${lmfit_SOURCE_DIR}/lib)
    #    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} lmfit)

    project(lmfit LANGUAGES C)
    set(CMAKE_C_STANDARD 99)

    include_directories ("thirdParty/lmfit/lib")
    set(src_files
            "thirdParty/lmfit/lib/lmcurve.c"
            "thirdParty/lmfit/lib/lmmin.c"
            "thirdParty/lmfit/lib/lminvert.c")

    add_library(lmfit ${src_files})
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} lmfit)

endif(USE_LMFIT)