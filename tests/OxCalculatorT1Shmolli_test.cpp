/*!
 * \file OxCalculatorT1Shmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/02
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxSignCalculatorNoSign.h"
#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorDefault3Dims.h"

#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1Shmolli.h"
#include "OxFunctionsT1Shmolli.h"
#include "OxFitterAmoebaPrivateNr2.h"
#include "OxSignCalculatorShmolli.h"
#include "OxStartPointCalculatorShmolli.h"

TEST(OxCalculatorT1Shmolli, calculate_throwIfIncorrectNumberOfSamples) {

    typedef double TYPE;

    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;
    calculatorT1Shmolli.setNSamples(8);

    EXPECT_THROW(calculatorT1Shmolli.calculate(), std::exception);
}

#ifdef USE_VNL
TEST(OxCalculatorT1Shmolli, calculate_ICE) {

    typedef double TYPE;

    char filePath [] = "testData/ice.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Shmolli<TYPE> functionsObject;
    Ox::FitterAmoebaPrivateNr2<TYPE> fitter;
    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;

    // configure
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Shmolli.calculate();

    EXPECT_NEAR(calculatorT1Shmolli.getResults().A, 42.88, 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().B, -21.82, 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().T1star, 511.3, 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Shmolli, calculate_doNotCalculateIfMaxIterZero) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;

    // configure
    fitter.setMaxFunctionEvals(0); // I set the maxFunctionEvals to zero
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());

    calculatorT1Shmolli.calculate();

    EXPECT_DOUBLE_EQ(calculatorT1Shmolli.getResults().A, 0);
    EXPECT_DOUBLE_EQ(calculatorT1Shmolli.getResults().B, 0);
    EXPECT_DOUBLE_EQ(calculatorT1Shmolli.getResults().T1star, 0);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Shmolli, calculate_throwIfInvTimesNotSorted) {

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
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;

    // configure
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(invTimesNotSorted);
    calculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());

    EXPECT_THROW(calculatorT1Shmolli.calculate(), std::runtime_error);

    delete [] invTimesNotSorted;
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Shmolli, calculate_WithoutSigns_AmoebaVnl) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;

    // configure
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());

    calculatorT1Shmolli.calculate();

    EXPECT_NEAR(calculatorT1Shmolli.getResults().A, testData.getResultsShmolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().B, testData.getResultsShmolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().T1star, testData.getResultsShmolli()[2], 1e-2);

}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Shmolli, calculate_WithoutSigns_LevMarVnl) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;

    // configure
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());

    calculatorT1Shmolli.calculate();

    EXPECT_NEAR(calculatorT1Shmolli.getResults().A, testData.getResultsShmolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().B, testData.getResultsShmolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().T1star, testData.getResultsShmolli()[2], 1e-2);

}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Shmolli, calculate_WithSigns) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;

    // configure
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());

    calculatorT1Shmolli.calculate();

    EXPECT_NEAR(calculatorT1Shmolli.getResults().A, testData.getResultsShmolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().B, testData.getResultsShmolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Shmolli.getResults().T1star, testData.getResultsShmolli()[2], 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1Shmolli, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;

    // configure
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    calculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    calculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());
    calculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    
    Ox::CalculatorT1Shmolli<TYPE> calculatorT1ShmolliCopy = calculatorT1Shmolli;

    EXPECT_EQ( calculatorT1Shmolli.getMeanCutOff(), calculatorT1ShmolliCopy.getMeanCutOff());
    EXPECT_EQ( calculatorT1Shmolli.getNSamples(), calculatorT1ShmolliCopy.getNSamples());
    EXPECT_EQ( calculatorT1Shmolli.getNDims(), calculatorT1ShmolliCopy.getNDims());

    // empty object pointers
    EXPECT_THROW(calculatorT1ShmolliCopy.getFunctionsT1(), std::runtime_error);
    EXPECT_THROW(calculatorT1ShmolliCopy.getFitter(), std::runtime_error);
    EXPECT_THROW(calculatorT1ShmolliCopy.getSignCalculator(), std::runtime_error);
    EXPECT_THROW(calculatorT1ShmolliCopy.getStartPointCalculator(), std::runtime_error);

    // empty array pointers
    EXPECT_THROW(calculatorT1ShmolliCopy.getInvTimes(), std::runtime_error);
    EXPECT_FALSE(calculatorT1ShmolliCopy.getRepTimes());
    EXPECT_FALSE(calculatorT1ShmolliCopy.getRelAcqTimes());
    EXPECT_THROW(calculatorT1ShmolliCopy.getSigMag(), std::runtime_error);
    EXPECT_FALSE(calculatorT1ShmolliCopy.getSigPha());

    // non-empty pointers of internal arrays
    EXPECT_TRUE(calculatorT1ShmolliCopy.getSignal());
    EXPECT_TRUE(calculatorT1ShmolliCopy.getSigns());
    EXPECT_TRUE(calculatorT1ShmolliCopy.getStartPoint());
}
#endif

#endif // USE_PRIVATE_NR2