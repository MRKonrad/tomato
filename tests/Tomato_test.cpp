/*!
 * \file OxShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "gtest/gtest.h"
#include "Tomato.h"

TEST(ActeptanceTests_TomatoTest, readAndSortFileList) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoTest, readAndSortInputDirs) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_inputDirs.yaml");
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoTest, readAndSort_calculate) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoTest, readAndSort_calculate_export) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoTest, readAndSortFileList_calculate_export_noPhase) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList_noPhase.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoTest, readAndSortInputDirs_calculate_export_noPhase) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_inputDirs_noPhase.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

#ifdef USE_PRIVATE_NR2
TEST(ActeptanceTests_TomatoTest, readAndSortInputDirs_calculate_export_Philips) {

    Ox::Tomato<double> Tomato_object("testData/026_Hcmr_Phantom_Leeds_20131216_DAB_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS
    //Tomato_object.visualise();

}
#endif //USE_PRIVATE_NR2

#ifdef USE_PRIVATE_NR2
TEST(ActeptanceTests_TomatoTest, readAndSortInputDirs_calculate_export_GE) {

    Ox::Tomato<double> Tomato_object("testData/039-Hcmr_Phantom_Erasmus_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS
    //Tomato_object.visualise();

}
#endif //USE_PRIVATE_NR2

#endif // USE_ITK