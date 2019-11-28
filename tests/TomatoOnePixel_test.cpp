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
TEST(TomatoOnePixel, blood) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);

    Ox::TomatoOptions<TYPE> options;
    options.signal_magnitude = testData.getSignalMag();
    options.echo_times = testData.getEchoTimes();
    options.parameter_to_map = Ox::T2_basic;
    options.model_type = Ox::_ModelT2TwoParam;

//    options.fitting_method = Ox::LevMarVnl;
//    options.printCurrent();
    std::map<std::string, TYPE > results = Ox::calculateOnePixel(options);

    EXPECT_NEAR(results["A"],  testData.getResultsTwoParam()[0], 1e-1);
    EXPECT_NEAR(results["T2"], testData.getResultsTwoParam()[1], 1e-1);
}

#endif