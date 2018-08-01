/*!
 * \file OxSignCalculatorRealImag_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxSignCalculatorRealImag.h"

TEST(SignCalculatorRealImag, calculateSign) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};
    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    signCalculator.setNSamples(testData.getNSamples());
    signCalculator.setSigMag(testData.getSignalMagPtr());
    signCalculator.setSigPha(testData.getSignalPhaPtr());
    signCalculator.setInvTimes(testData.getInvTimesPtr());
    signCalculator.setSignal(signal);
    signCalculator.setSigns(signs);

    signCalculator.calculateSign();

    for (int iSample = 0; iSample < nSamples; iSample++) {
        EXPECT_EQ(testData.getSigns()[iSample], signs[iSample]);
    }
}

TEST(SignCalculatorRealImag, calculateSign_throw) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};
    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    signCalculator.setNSamples(testData.getNSamples());
    signCalculator.setSigMag(testData.getSignalMagPtr());
    signCalculator.setSigPha(testData.getSignalPhaPtr());
    // signCalculator.setInvTimes(testData.getInvTimesPtr());
    signCalculator.setSignal(signal);
    signCalculator.setSigns(signs);

    EXPECT_ANY_THROW(signCalculator.calculateSign());

}



