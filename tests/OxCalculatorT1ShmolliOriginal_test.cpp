/*!
 * \file OxCalculatorT1ShmolliOriginal_test.cpp
 * \author Konrad Werys
 * \date 2019/08/15
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK
#ifdef USE_PRIVATE_NR2
#ifdef USE_YAML

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxCalculatorT1ShmolliOriginal.h"

TEST(OxCalculatorT1ShmolliOriginal, blood_test) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    Ox::CalculatorT1ShmolliOriginal<TYPE> calculatorT1ShmolliOriginal;

    // set the data
    calculatorT1ShmolliOriginal.setNSamples(nSamples);
    calculatorT1ShmolliOriginal.setInvTimes(testData.getInvTimesPtr());
    calculatorT1ShmolliOriginal.setSigPha(testData.getSignalPhaPtr());
    calculatorT1ShmolliOriginal.setSigMag(testData.getSignalMagPtr());
    calculatorT1ShmolliOriginal.setInvTimes(testData.getInvTimesPtr());
    calculatorT1ShmolliOriginal.setMeanCutOff(10);

    calculatorT1ShmolliOriginal.calculate();

    EXPECT_NEAR(calculatorT1ShmolliOriginal.getResults()["A"], testData.getResultsShmolli()[0], 1e-0);
    EXPECT_NEAR(calculatorT1ShmolliOriginal.getResults()["B"], testData.getResultsShmolli()[1], 1e-0);
    EXPECT_NEAR(calculatorT1ShmolliOriginal.getResults()["T1star"], testData.getResultsShmolli()[2], 1e-0);

}

TEST(OxCalculatorT1ShmolliOriginal, myocardium_test) {

    typedef double TYPE;

    char filePath [] = "testData/T1_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    Ox::CalculatorT1ShmolliOriginal<TYPE> calculatorT1ShmolliOriginal;

    // set the data
    calculatorT1ShmolliOriginal.setNSamples(nSamples);
    calculatorT1ShmolliOriginal.setInvTimes(testData.getInvTimesPtr());
    calculatorT1ShmolliOriginal.setSigPha(testData.getSignalPhaPtr());
    calculatorT1ShmolliOriginal.setSigMag(testData.getSignalMagPtr());
    calculatorT1ShmolliOriginal.setInvTimes(testData.getInvTimesPtr());
    calculatorT1ShmolliOriginal.setMeanCutOff(10);

    calculatorT1ShmolliOriginal.calculate();

    EXPECT_NEAR(calculatorT1ShmolliOriginal.getResults()["A"], testData.getResultsShmolli()[0], 1e-0);
    EXPECT_NEAR(calculatorT1ShmolliOriginal.getResults()["B"], testData.getResultsShmolli()[1], 1e-0);
    EXPECT_NEAR(calculatorT1ShmolliOriginal.getResults()["T1star"], testData.getResultsShmolli()[2], 1e-0);

}

#endif // USE_YAML
#endif // USE_PRIVATE_NR2
#endif // USE_ITK