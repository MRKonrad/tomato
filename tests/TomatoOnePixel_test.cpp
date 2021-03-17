/*!
 * \file OxCalculatorT2_test.cpp
 * \author Konrad Werys
 * \date 2019/11/01
 */

#include <OxFitterLevenbergMarquardtVnl.h>
#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "TomatoOnePixel.h"

#ifdef USE_VNL
#ifdef USE_YAML

TEST(TomatoOnePixel, T1_blood) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);

    Ox::TomatoOptions<TYPE> options;
    options.signal_magnitude = testData.getSignalMag();
    options.inversion_times = testData.getInvTimes();
    options.parameter_to_map = Ox::T1_MOLLI;
    options.model_type = Ox::_ModelT1ThreeParam;

    std::map<std::string, TYPE > results = Ox::calculateOnePixel(options);

    EXPECT_NEAR(results["A"],  testData.getResultsMolli()[0], 1e-1);
    EXPECT_NEAR(results["B"], testData.getResultsMolli()[1], 1e-1);
    EXPECT_NEAR(results["T1star"], testData.getResultsMolli()[2], 1e-1);
}

TEST(TomatoOnePixel, T1_long) {

    typedef double TYPE;

    char filePath [] = "testData/T1_long.yaml";
    Ox::TestData<TYPE> testData(filePath);

    Ox::TomatoOptions<TYPE> options;
    options.signal_magnitude = testData.getSignalMag();
    options.inversion_times = testData.getInvTimes();
    options.parameter_to_map = Ox::T1_MOLLI;
    options.model_type = Ox::_ModelT1ThreeParam;

    std::map<std::string, TYPE > results = Ox::calculateOnePixel(options);

    EXPECT_NEAR(results["A"],  testData.getResultsMolli()[0], 1e-1);
    EXPECT_NEAR(results["B"], testData.getResultsMolli()[1], 1e-1);
    EXPECT_NEAR(results["T1star"], testData.getResultsMolli()[2], 1e-1);
}

#ifdef USE_PRIVATE_NR2
TEST(TomatoOnePixel, T2_blood) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);

    Ox::TomatoOptions<TYPE> options;
    options.signal_magnitude = testData.getSignalMag();
    options.echo_times = testData.getEchoTimes();
    options.parameter_to_map = Ox::T2_basic;
    options.model_type = Ox::_ModelT2TwoParam;

    std::map<std::string, TYPE > results = Ox::calculateOnePixel(options);

    EXPECT_NEAR(results["A"],  testData.getResultsTwoParam()[0], 2e-1);
    EXPECT_NEAR(results["T2"], testData.getResultsTwoParam()[1], 2e-1);
}
#endif // USE_PRIVATE_NR2

#endif // USE_YAML
#endif // USE_VNL