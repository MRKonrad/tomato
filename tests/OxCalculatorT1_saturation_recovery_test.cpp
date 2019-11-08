/*!
 * \file OxCalculatorT1Molli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include <OxFitterLevenbergMarquardtVnl.h>
#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "OxModelT1TwoParam.h"
#include "OxModelT1ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT1Molli.h"

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
    Ox::ModelT1TwoParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure
    calculatorT1Molli.setModel(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Molli.setNSamples(nSamples);
    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults()["A"], testData.getResultsTwoParam()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults()["T1"], testData.getResultsTwoParam()[1], 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1_saturation_recovery, threeParam) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure
    calculatorT1Molli.setModel(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Molli.setNSamples(nSamples);
    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults()["A"], testData.getResultsThreeParam()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults()["B"], testData.getResultsThreeParam()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults()["T1star"], testData.getResultsThreeParam()[2], 1e-2);
}
#endif



