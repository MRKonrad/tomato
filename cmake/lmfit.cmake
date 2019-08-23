if(USE_LMFIT)

    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_STANDARD 99)

    include_directories ("thirdParty/lmfit/include")
    link_directories(${CMAKE_SOURCE_DIR}/thirdParty/lmfit/lib)
    set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} lmfit)
    if (WIN32)
        install (FILES thirdParty/lmfit/bin/lmfit.dll DESTINATION bin)
        install (FILES thirdParty/lmfit/bin/lmfit.dll DESTINATION tests)
    endif()

endif(USE_LMFIT)