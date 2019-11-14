/*!
 * \file TomatoT2_test.cpp
 * \author Konrad Werys
 * \date 2019/11/12
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "gtest/gtest.h"
#include "Tomato.h"

TEST(ActeptanceTests_TomatoT2Test, readAndSortInputDirs) {

    Ox::Tomato<double> Tomato_object("testData/tomatoConfig_T2_inputDirPaths.yaml");
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoT2Test, readAndSort_calculate) {

    Ox::Tomato<double> Tomato_object("testData/tomatoConfig_T2_inputDirPaths.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoT2Test, readAndSort_calculate_export) {

    Ox::Tomato<double> Tomato_object("testData/tomatoConfig_T2_inputDirPaths.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

#endif // USE_ITK