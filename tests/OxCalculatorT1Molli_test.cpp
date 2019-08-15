/*!
 * \file OxCalculatorT1Molli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include <OxFitterLevenbergMarquardtVnl.h>
#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorDefault3Dims.h"
#include "OxCalculatorT1Molli.h"

#ifdef USE_ITK
#include "itkTimeProbe.h"
#endif

//TODO: make sure correctSDs are accually correct

#ifdef USE_VNL
TEST(OxCalculatorT1Molli, calculate_doNotCalculateIfMaxIterZero) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> CalculatorT1Molli;

    // configure
    fitterAmoebaVnl.setMaxFunctionEvals(0); // I set the maxFunctionEvals to zero
    CalculatorT1Molli.setFunctionsT1(&functionsObject);
    CalculatorT1Molli.setFitter(&fitterAmoebaVnl);
    CalculatorT1Molli.setSignCalculator(&signCalculator);
    CalculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // set the data
    CalculatorT1Molli.setNSamples(nSamples);
    CalculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
    CalculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
    CalculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    CalculatorT1Molli.calculate();

    EXPECT_DOUBLE_EQ(CalculatorT1Molli.getResults().A, 0);
    EXPECT_DOUBLE_EQ(CalculatorT1Molli.getResults().B, 0);
    EXPECT_DOUBLE_EQ(CalculatorT1Molli.getResults().T1star, 0);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Molli, calculate_throwIfInvTimesNotSorted) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE *invTimesNotSorted = new TYPE[nSamples];
    invTimesNotSorted[0] = testData.getSignalPhaPtr()[1];
    invTimesNotSorted[1] = testData.getSignalPhaPtr()[0];
    invTimesNotSorted[2] = testData.getSignalPhaPtr()[2];
    invTimesNotSorted[3] = testData.getSignalPhaPtr()[3];
    invTimesNotSorted[4] = testData.getSignalPhaPtr()[4];
    invTimesNotSorted[5] = testData.getSignalPhaPtr()[5];
    invTimesNotSorted[6] = testData.getSignalPhaPtr()[6];

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> CalculatorT1Molli;

    // configure
    CalculatorT1Molli.setFunctionsT1(&functionsObject);
    CalculatorT1Molli.setFitter(&fitterAmoebaVnl);
    CalculatorT1Molli.setSignCalculator(&signCalculator);
    CalculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // set the data
    CalculatorT1Molli.setNSamples(nSamples);
    CalculatorT1Molli.setInvTimes(invTimesNotSorted);
    CalculatorT1Molli.setSigPha(testData.getSignalMagPtr());
    CalculatorT1Molli.setSigMag(testData.getSignalPhaPtr());

    EXPECT_THROW(CalculatorT1Molli.calculate(), std::runtime_error);

    delete [] invTimesNotSorted;
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Molli, calculate_WithoutSigns) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
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
    calculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults().A, testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().B, testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().T1star, testData.getResultsMolli()[2], 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Molli, calculate_WithSigns) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
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
    calculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults().A, testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().B, testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().T1star, testData.getResultsMolli()[2], 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Molli, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
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
    calculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.setMeanCutOff(123);

    Ox::CalculatorT1Molli<TYPE> calculatorT1MolliCopy = calculatorT1Molli;

    EXPECT_EQ( calculatorT1Molli.getMeanCutOff(), calculatorT1MolliCopy.getMeanCutOff());
    EXPECT_EQ( calculatorT1Molli.getNSamples(), calculatorT1MolliCopy.getNSamples());
    EXPECT_EQ( calculatorT1Molli.getNDims(), calculatorT1MolliCopy.getNDims());

    // empty object pointers
    EXPECT_THROW(calculatorT1MolliCopy.getFunctionsT1(), std::runtime_error);
    EXPECT_THROW(calculatorT1MolliCopy.getFitter(), std::runtime_error);
    EXPECT_THROW(calculatorT1MolliCopy.getSignCalculator(), std::runtime_error);
    EXPECT_THROW(calculatorT1MolliCopy.getStartPointCalculator(), std::runtime_error);

    // empty array pointers
    EXPECT_THROW(calculatorT1MolliCopy.getInvTimes(), std::runtime_error);
    EXPECT_FALSE(calculatorT1MolliCopy.getRepTimes());
    EXPECT_FALSE(calculatorT1MolliCopy.getRelAcqTimes());
    EXPECT_THROW(calculatorT1MolliCopy.getSigMag(), std::runtime_error);
    EXPECT_FALSE(calculatorT1MolliCopy.getSigPha());

    // non-empty pointers of internal arrays
    EXPECT_TRUE(calculatorT1MolliCopy.getSignal());
    EXPECT_TRUE(calculatorT1MolliCopy.getSigns());
    EXPECT_TRUE(calculatorT1MolliCopy.getStartPoint());

}
#endif

#ifdef USE_VNL

TEST(OxCalculatorT1Molli, correctSDs) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
    calculatorT1Molli.setDoCalculateSDMap(true);

    // set the data
    calculatorT1Molli.setNSamples(nSamples);
    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Molli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Molli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults().SD_T1, 48.31, 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().SD_A, 1.96, 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().SD_B, 2.07, 1e-2);
}

#endif

////TODO: no difference between 5 samples and 7 samples here.
//#ifdef USE_ITK
//
//TEST(OxCalculatorT1Molli, samples5) {
//
//    typedef double TYPE;
//    int nRepetitions = 10000;
//
//    char filePath5 [] = "testData/blood_5samples.yaml";
//    Ox::TestData<TYPE> testData5(filePath5);
//    int nSamples5 = testData5.getNSamples();
//
//    char filePath7 [] = "testData/blood.yaml";
//    Ox::TestData<TYPE> testData7(filePath7);
//    int nSamples7 = testData7.getNSamples();
//
//    // init the necessary objects
//    Ox::FunctionsT1Basic<TYPE> functionsObject;
//    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterAmoebaVnl;
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
//    calculatorT1Molli.setNSamples(nSamples5);
//    calculatorT1Molli.setInvTimes(testData5.getInvTimesPtr());
//    calculatorT1Molli.setSigPha(testData5.getSignalPhaPtr());
//    calculatorT1Molli.setSigMag(testData5.getSignalMagPtr());
//
//    printf("\n");
//    itk::TimeProbe clock;
//    clock.Start();
//    for (int i = 0; i < nRepetitions; i++)
//        calculatorT1Molli.calculate();
//    clock.Stop();
//    printf("Calculation5 time: %.12fs.\n", clock.GetTotal());
//
//    // set the data
//    calculatorT1Molli.setNSamples(nSamples7);
//    calculatorT1Molli.setInvTimes(testData7.getInvTimesPtr());
//    calculatorT1Molli.setSigPha(testData7.getSignalPhaPtr());
//    calculatorT1Molli.setSigMag(testData7.getSignalMagPtr());
//
//    clock.Reset();
//    clock.Start();
//    for (int i = 0; i < nRepetitions; i++)
//        calculatorT1Molli.calculate();
//    clock.Stop();
//    printf("Calculation7 time: %.12fs.\n", clock.GetTotal());
//
//}
//
//#endif
