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

TEST(OxCalculatorT1Molli, calculateWithoutSigns) {

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
    calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());

    calculatorT1Molli.calculate();

    EXPECT_NEAR(calculatorT1Molli.getResults()[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults()[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults()[2], testData.getResultsMolli()[2], 1e-2);
}

TEST(OxCalculatorT1Molli, calculateWithSigns) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    //double results[3] = {0, 0, 0};

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

    EXPECT_NEAR(calculatorT1Molli.getResults()[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults()[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1Molli.getResults()[2], testData.getResultsMolli()[2], 1e-2);
}

