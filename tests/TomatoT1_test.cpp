/*!
 * \file TomatoT1_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "gtest/gtest.h"
#include "Tomato.h"

TEST(ActeptanceTests_TomatoT1Test, readAndSortFileList) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoT1Test, readAndSortInputDirs) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_inputDirs.yaml");
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

#ifdef USE_LMFIT
TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export_LevMarLmfit) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    Tomato_object._opts->fitting_method = Ox::LevMarLmfit;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}
#endif

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export_AmoebaVnl) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    Tomato_object._opts->fitting_method = Ox::AmoebaVnl;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

#ifdef USE_PRIVATE_NR2
TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export_T1_SHMOLLI_original) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    Tomato_object._opts->parameter_to_map = Ox::T1_SHMOLLI_original;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}
#endif

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_incorrect_calculator) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->parameter_to_map = (Ox::calculatorsType_t)118;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_THROW(Tomato_object.calculate(), std::runtime_error);

}

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export_incorrect_fitter) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->fitting_method = (Ox::fittersType_t)118;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_THROW(Tomato_object.calculate(), std::runtime_error);

}

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export_incorrect_functions) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->functions_type = (Ox::functionsType_t)118;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_THROW(Tomato_object.calculate(), std::runtime_error);

}

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export_incorrect_signCalculator) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->sign_calc_method = (Ox::signCalculatorsType_t)118;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_THROW(Tomato_object.calculate(), std::runtime_error);

}

TEST(ActeptanceTests_TomatoT1Test, readAndSort_calculate_export_incorrect_startPointCalulator) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    Tomato_object._opts->start_point_calc_method = (Ox::startPointCalculatorsType_t)118;
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_THROW(Tomato_object.calculate(), std::runtime_error);

}

////TODO: why does it take so long in case of 5 samples, but not 6 or 7 samples
//TEST(ActeptanceTests_TomatoT1Test, readAndSort_5samples) {
//
//    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
//    Tomato_object._opts->files_magnitude.pop_back();
//    Tomato_object._opts->files_magnitude.pop_back();
//    Tomato_object._opts->files_phase.pop_back();
//    Tomato_object._opts->files_phase.pop_back();
//    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
//    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
//    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS
//}

TEST(ActeptanceTests_TomatoT1Test, readAndSortFileList_calculate_export_noPhase) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList_noPhase.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

TEST(ActeptanceTests_TomatoT1Test, readAndSortInputDirs_calculate_export_noPhase) {

    Ox::Tomato<double> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_inputDirs_noPhase.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}

#ifdef USE_PRIVATE_NR2
TEST(ActeptanceTests_TomatoT1Test, readAndSortInputDirs_calculate_export_Philips) {

    Ox::Tomato<double> Tomato_object("testData/026_Hcmr_Phantom_Leeds_20131216_DAB_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}
#endif //USE_PRIVATE_NR2

#ifdef USE_PRIVATE_NR2
TEST(ActeptanceTests_TomatoT1Test, readAndSortInputDirs_calculate_export_GE) {

    Ox::Tomato<double> Tomato_object("testData/039-Hcmr_Phantom_Erasmus_fileList.yaml");
    Tomato_object._opts->max_function_evals = 5; // to make the calculations faster
    EXPECT_EQ(Tomato_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.calculate(), 0); // EXIT_SUCCESS
    EXPECT_EQ(Tomato_object.exportToDicom(), 0); // EXIT_SUCCESS

}
#endif //USE_PRIVATE_NR2

#endif // USE_ITK