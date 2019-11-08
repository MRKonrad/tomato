/*!
 * \file OxCalculatorT2_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include <OxFitterLevenbergMarquardtVnl.h>
#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "OxModelT2ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT2.h"

#ifdef USE_ITK
#include "itkTimeProbe.h"
#endif

#ifdef USE_VNL
TEST(OxCalculatorT2, blood) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2<TYPE> calculatorT2;

    // configure
    TYPE startPoint[] = {1, 100, 50};
    startPointCalculator.setInputStartPoint(startPoint);
    calculatorT2.setStartPointCalculator(&startPointCalculator);
    calculatorT2.setModel(&functionsObject);
    calculatorT2.setFitter(&fitterAmoebaVnl);

    // set the data
    calculatorT2.setNSamples(nSamples);
    calculatorT2.setEchoTimes(testData.getEchoTimesPtr());
    calculatorT2.setSigMag(testData.getSignalMagPtr());

    calculatorT2.calculate();

    EXPECT_NEAR(calculatorT2.getResults()["A"],  testData.getResultsThreeParam()[0], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["B"],  testData.getResultsThreeParam()[1], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["T2"], testData.getResultsThreeParam()[2], 1e-1);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT2, myo) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> functionsObject;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterAmoebaVnl;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2<TYPE> calculatorT2;

    // configure
    TYPE startPoint[] = {1, 100, 50};
    startPointCalculator.setInputStartPoint(startPoint);
    calculatorT2.setStartPointCalculator(&startPointCalculator);
    calculatorT2.setModel(&functionsObject);
    calculatorT2.setFitter(&fitterAmoebaVnl);


    // set the data
    calculatorT2.setNSamples(nSamples);
    calculatorT2.setEchoTimes(testData.getEchoTimesPtr());
    calculatorT2.setSigMag(testData.getSignalMagPtr());

    calculatorT2.calculate();

    EXPECT_NEAR(calculatorT2.getResults()["A"],  testData.getResultsThreeParam()[0], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["B"],  testData.getResultsThreeParam()[1], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["T2"], testData.getResultsThreeParam()[2], 1e-1);
}
#endif