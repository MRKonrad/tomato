# TODO: bring it to the right form
if(${USE_TOMATOFIT})
    if (EXISTS ${PROJECT_SOURCE_DIR}/../tomato_fit/cpp_src AND EXISTS ${PROJECT_SOURCE_DIR}/../tomato_fit/thirdParty/eigen)
        include_directories(${PROJECT_SOURCE_DIR}/../tomato_fit/cpp_src)
        include_directories(${PROJECT_SOURCE_DIR}/../tomato_fit/thirdParty/eigen)
    else()
        set(USE_TOMATOFIT OFF)
    endif()
endif()