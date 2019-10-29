/*!
 * \file OxCalculatorT1Molli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include <OxFitterLevenbergMarquardtVnl.h>
#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "OxFunctionsT1TwoParam.h"
#include "OxFunctionsT1ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxStartPointCalculatorDefault3Dims.h"
#include "OxCalculatorT1Molli.h"

#ifdef USE_ITK
#include "itkTimeProbe.h"
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1_saturation_recovery, twoParam) {

    typedef double TYPE;

    char filePath [] = "testData/saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1TwoParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Molli.setNSamples(nSamples);
    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults().A, testData.getResultsTwoParam()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().B, testData.getResultsTwoParam()[1], 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1_saturation_recovery, threeParam) {

    typedef double TYPE;

    char filePath [] = "testData/saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1ThreeParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Molli.setNSamples(nSamples);
    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults().A, testData.getResultsThreeParam()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().B, testData.getResultsThreeParam()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().T1star, testData.getResultsThreeParam()[2], 1e-2);
}
#endif

//
//#ifdef USE_VNL
//TEST(OxCalculatorT1Molli, calculate_WithSigns) {
//
//    typedef double TYPE;
//
//    char filePath [] = "testData/blood.yaml";
//    Ox::TestData<TYPE> testData(filePath);
//    int nSamples = testData.getNSamples();
//
//    // init the necessary objects
//    Ox::FunctionsT1ThreeParam<TYPE> functionsObject;
//    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
//    Ox::SignCalculatorRealImag<TYPE> signCalculator;
//    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
//    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;
//
//    // configure
//    calculatorT1Molli.setFunctionsT1(&functionsObject);
//    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
//    calculatorT1Molli.setSignCalculator(&signCalculator);
//    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
//
//    // set the data
//    calculatorT1Molli.setNSamples(nSamples);
//    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
//    calculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
//    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());
//
//    calculatorT1Molli.calculate();
//
//    EXPECT_NEAR(calculatorT1Molli.getResults().A, testData.getResultsMolli()[0], 1e-2);
//    EXPECT_NEAR(calculatorT1Molli.getResults().B, testData.getResultsMolli()[1], 1e-2);
//    EXPECT_NEAR(calculatorT1Molli.getResults().T1star, testData.getResultsMolli()[2], 1e-2);
//}
//#endif
//
//#ifdef USE_VNL
//TEST(OxCalculatorT1Molli, copyConstructor) {
//
//    typedef double TYPE;
//
//    char filePath [] = "testData/blood.yaml";
//    Ox::TestData<TYPE> testData(filePath);
//    int nSamples = testData.getNSamples();
//
//    // init the necessary objects
//    Ox::FunctionsT1ThreeParam<TYPE> functionsObject;
//    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
//    Ox::SignCalculatorRealImag<TYPE> signCalculator;
//    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
//    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;
//
//    // configure
//    calculatorT1Molli.setFunctionsT1(&functionsObject);
//    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
//    calculatorT1Molli.setSignCalculator(&signCalculator);
//    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
//
//    // set the data
//    calculatorT1Molli.setNSamples(nSamples);
//    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
//    calculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
//    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());
//
//    calculatorT1Molli.setMeanCutOff(123);
//
//    Ox::CalculatorT1Molli<TYPE> calculatorT1MolliCopy = calculatorT1Molli;
//
//    EXPECT_EQ( calculatorT1Molli.getMeanCutOff(), calculatorT1MolliCopy.getMeanCutOff());
//    EXPECT_EQ( calculatorT1Molli.getNSamples(), calculatorT1MolliCopy.getNSamples());
//    EXPECT_EQ( calculatorT1Molli.getNDims(), calculatorT1MolliCopy.getNDims());
//
//    // empty object pointers
//    EXPECT_THROW(calculatorT1MolliCopy.getFunctionsT1(), std::runtime_error);
//    EXPECT_THROW(calculatorT1MolliCopy.getFitter(), std::runtime_error);
//    EXPECT_THROW(calculatorT1MolliCopy.getSignCalculator(), std::runtime_error);
//    EXPECT_THROW(calculatorT1MolliCopy.getStartPointCalculator(), std::runtime_error);
//
//    // empty array pointers
//    EXPECT_THROW(calculatorT1MolliCopy.getInvTimes(), std::runtime_error);
//    EXPECT_FALSE(calculatorT1MolliCopy.getRepTimes());
//    EXPECT_FALSE(calculatorT1MolliCopy.getRelAcqTimes());
//    EXPECT_THROW(calculatorT1MolliCopy.getSigMag(), std::runtime_error);
//    EXPECT_FALSE(calculatorT1MolliCopy.getSigPha());
//
//    // non-empty pointers of internal arrays
//    EXPECT_TRUE(calculatorT1MolliCopy.getSignal());
//    EXPECT_TRUE(calculatorT1MolliCopy.getSigns());
//    EXPECT_TRUE(calculatorT1MolliCopy.getStartPoint());
//
//}
//#endif
//
//#ifdef USE_VNL
//
//TEST(OxCalculatorT1Molli, correctSDs) {
//
//    typedef double TYPE;
//
//    char filePath [] = "testData/blood.yaml";
//    Ox::TestData<TYPE> testData(filePath);
//    int nSamples = testData.getNSamples();
//
//    // init the necessary objects
//    Ox::FunctionsT1ThreeParam<TYPE> functionsObject;
//    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
//    Ox::SignCalculatorRealImag<TYPE> signCalculator;
//    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
//    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;
//
//    // configure
//    calculatorT1Molli.setFunctionsT1(&functionsObject);
//    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
//    calculatorT1Molli.setSignCalculator(&signCalculator);
//    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
//    calculatorT1Molli.setDoCalculateSDMap(true);
//
//    // set the data
//    calculatorT1Molli.setNSamples(nSamples);
//    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
//    calculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
//    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());
//
//    calculatorT1Molli.calculate();
//
//    EXPECT_NEAR(calculatorT1Molli.getResults().SD_T1, 48.31, 1e-2);
//    EXPECT_NEAR(calculatorT1Molli.getResults().SD_A, 1.96, 1e-2);
//    EXPECT_NEAR(calculatorT1Molli.getResults().SD_B, 2.07, 1e-2);
//}
//
//#endif


