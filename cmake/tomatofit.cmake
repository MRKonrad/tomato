# TODO: bring it to the right form
if(${USE_TOMATOFIT})
    if (EXISTS /Users/kwerys/Code/tomato_fit/cpp_src AND EXISTS /Users/kwerys/Code/tomato_fit/thirdParty/eigen)
        include_directories(/Users/kwerys/Code/tomato_fit/cpp_src)
        include_directories(/Users/kwerys/Code/tomato_fit/thirdParty/eigen)
    else()
        set(USE_TOMATOFIT OFF)
    endif()
endif()