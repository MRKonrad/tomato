/*!
 * \file OxSignCalculatorRealImag_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxSignCalculatorRealImag.h"

#ifndef _WIN32
TEST(OxSignCalculatorRealImag, disp) {

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

    testing::internal::CaptureStdout();
    signCalculator.disp();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.size(), 0);
}

#endif // _WIN32


TEST(OxSignCalculatorRealImag, calculateSign_blood) {

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

TEST(OxSignCalculatorRealImag, calculateSign_myocardium) {

    typedef double TYPE;

    char filePath [] = "testData/myocardium.yaml";
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

TEST(OxSignCalculatorRealImag, calculateSign_throwIfSignalPhaNotSet) {

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

TEST(OxSignCalculatorRealImag, copyConstructor) {

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



