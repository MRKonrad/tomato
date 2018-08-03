
if(USE_NR3)
    SET(NR3_RECIPES_DIR  ${CMAKE_SOURCE_DIR}/../NumericalRecipes3/code)
    if(EXISTS ${NR3_RECIPES_DIR})
        include_directories (${NR3_RECIPES_DIR})
        # TODO: build NR3
        set(OxSmolli2_LIBS_TO_LINK ${OxSmolli2_LIBS_TO_LINK} NR2)
        message("Using NR3")
    else()
        set(USE_NR3 OFF)
    endif()
endif(USE_NR3)