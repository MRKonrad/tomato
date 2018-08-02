/*!
 * \file OxCalculatorT1Molli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorRealImag.h"
#include "OxCalculatorT1Molli.h"

TEST(OxCalculatorT1Molli, calculate_doNotCalculateIfMaxIterZero) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculator<TYPE> signCalculator;
    Ox::StartPointCalculator<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> CalculatorT1Shmolli;

    // configure
    fitterAmoebaVnl.setMaxFunctionEvals(0); // I set the maxFunctionEvals to zero
    CalculatorT1Shmolli.setFunctionsT1(&functionsObject);
    CalculatorT1Shmolli.setFitter(&fitterAmoebaVnl);
    CalculatorT1Shmolli.setSignCalculator(&signCalculator);
    CalculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    CalculatorT1Shmolli.setNSamples(nSamples);
    CalculatorT1Shmolli.setInvTimes(testData.getInvTimesPtr());
    CalculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    CalculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());

    CalculatorT1Shmolli.calculate();

    EXPECT_EQ(CalculatorT1Shmolli.getResults().A, 0);
    EXPECT_EQ(CalculatorT1Shmolli.getResults().B, 0);
    EXPECT_EQ(CalculatorT1Shmolli.getResults().T1star, 0);
}

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
    Ox::SignCalculator<TYPE> signCalculator;
    Ox::StartPointCalculator<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> CalculatorT1Shmolli;

    // configure
    CalculatorT1Shmolli.setFunctionsT1(&functionsObject);
    CalculatorT1Shmolli.setFitter(&fitterAmoebaVnl);
    CalculatorT1Shmolli.setSignCalculator(&signCalculator);
    CalculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    CalculatorT1Shmolli.setNSamples(nSamples);
    CalculatorT1Shmolli.setInvTimes(invTimesNotSorted);
    CalculatorT1Shmolli.setSigPha(testData.getSignalPhaPtr());
    CalculatorT1Shmolli.setSigMag(testData.getSignalMagPtr());

    EXPECT_THROW(CalculatorT1Shmolli.calculate(), std::runtime_error);

    delete [] invTimesNotSorted;
}

TEST(OxCalculatorT1Molli, calculate_WithoutSigns) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculator<TYPE> signCalculator;
    Ox::StartPointCalculator<TYPE> startPointCalculator;
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

TEST(OxCalculatorT1Molli, calculate_WithSigns) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculator<TYPE> startPointCalculator;
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
    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults().A, testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().B, testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults().T1star, testData.getResultsMolli()[2], 1e-2);
}

