
if(USE_NR2)
#    set(NR2_RECIPES_DIR  "${CMAKE_SOURCE_DIR}/../NumericalRecipes3/legacy/nr2/C_211/recipes")
#    set(NR2_OTHER_DIR  "${CMAKE_SOURCE_DIR}/../NumericalRecipes3/legacy/nr2/C_211/other")

    set(NR2_RECIPES_DIR  "/Users/kwerys/Code/NumericalRecipes3/legacy/nr2/C_211/recipes")
    set(NR2_OTHER_DIR  "/Users/kwerys/Code/NumericalRecipes3/legacy/nr2/C_211/other")

    if(EXISTS ${NR2_RECIPES_DIR} AND EXISTS ${NR2_OTHER_DIR})
        include_directories (${PROJECT_SOURCE_DIR}/thirdParty/nr2)
        #include_directories (${NR2_RECIPES_DIR})
        # file (GLOB_RECURSE NR2_SOURCE_FILES ${NR2_RECIPES_DIR}/*.c*)
        set(NR2_SOURCE_FILES
#                ${NR2_OTHER_DIR}/nr.h
                ${NR2_OTHER_DIR}/nrutil.h
                ${NR2_OTHER_DIR}/nrutil.c
                ${PROJECT_SOURCE_DIR}/thirdParty/nr2/nr_modified.h
                ${NR2_RECIPES_DIR}/amoeba.c
                ${NR2_RECIPES_DIR}/amotry.c)
        message(${NR2_SOURCE_FILES})
        include_directories(${NR2_RECIPES_DIR} ${NR2_OTHER_DIR})
        add_library(NR2 ${NR2_SOURCE_FILES})
        set(OxSmolli2_LIBS_TO_LINK ${OxSmolli2_LIBS_TO_LINK} NR2)
        message("Using NR2")
    else()
        set(USE_NR2 OFF)
    endif()
endif(USE_NR2)