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

    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    signCalculator.setNSamples(testData.getNSamples());
    signCalculator.setSigMag(testData.getSignalMagPtr());
    signCalculator.setSigPha(testData.getSignalPhaPtr());
    signCalculator.setSignal(signal);
    signCalculator.setSigns(signs);

    signCalculator.calculateSign();

    for (int iSample = 0; iSample < nSamples; iSample++) {
        EXPECT_EQ(testData.getSigns()[iSample], signs[iSample]);
    }

    delete [] signal;
    delete [] signs;
}

TEST(SignCalculatorRealImag, calculateSign_throwIfSignalPhaNotSet) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    signCalculator.setNSamples(testData.getNSamples());
    signCalculator.setSigMag(testData.getSignalMagPtr());
    // signCalculator.setSigPha(testData.getSignalPhaPtr());
    signCalculator.setSignal(signal);
    signCalculator.setSigns(signs);

    EXPECT_THROW(signCalculator.calculateSign(), std::runtime_error);

    delete [] signal;
    delete [] signs;
}

TEST(SignCalculatorRealImag, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    signCalculator.setNSamples(testData.getNSamples());
    signCalculator.setSigMag(testData.getSignalMagPtr());
    signCalculator.setSigPha(testData.getSignalPhaPtr());
    signCalculator.setSignal(signal);
    signCalculator.setSigns(signs);

    Ox::SignCalculatorRealImag<TYPE> signCalculatorCopy(signCalculator);

    // same nSamples
    EXPECT_EQ(signCalculator.getNSamples(), signCalculatorCopy.getNSamples());

    // empty pointers
    EXPECT_THROW(signCalculatorCopy.getSigMag(), std::runtime_error);
    EXPECT_THROW(signCalculatorCopy.getSigPha(), std::runtime_error);
    EXPECT_THROW(signCalculatorCopy.getSignal(), std::runtime_error);
    EXPECT_THROW(signCalculatorCopy.getSigns(), std::runtime_error);

    delete [] signal;
    delete [] signs;
}



