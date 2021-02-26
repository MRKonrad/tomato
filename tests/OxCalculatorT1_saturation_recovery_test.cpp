/*!
 * \file OxCalculatorT1_saturation_recovery_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "OxModelT1TwoParam.h"
#include "OxModelT1ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxFitterLevenbergMarquardtLmfit.h"
#include "OxSignCalculatorNoSign.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT1WithSignCheck.h"

#ifdef USE_ITK
#include "itkTimeProbe.h"
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1_saturation_recovery, twoParam) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1TwoParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(testData.getInvTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], testData.getResultsTwoParam()[0], 1e-2);
    EXPECT_NEAR(calculator.getResults()["T1star"], testData.getResultsTwoParam()[1], 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1_saturation_recovery, threeParam) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(testData.getInvTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], testData.getResultsThreeParam()[0], 1e-2);
    EXPECT_NEAR(calculator.getResults()["B"], testData.getResultsThreeParam()[1], 1e-2);
    EXPECT_NEAR(calculator.getResults()["T1star"], testData.getResultsThreeParam()[2], 1e-2);
}
#endif



