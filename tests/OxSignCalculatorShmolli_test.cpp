/*!
 * \file OxSignCalculatorShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"
#include "OxTestData.h"

#ifdef USE_PRIVATE_NR2
#include "OxSignCalculatorShmolli.h"

TEST(OxSignCalculatorShmolli, calculateSign_blood) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};
    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    signCalculator.setInvTimes(testData.getInvTimesPtr());
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

TEST(OxSignCalculatorShmolli, calculateSign_myocardium) {

    typedef double TYPE;

    char filePath [] = "testData/myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};
    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    signCalculator.setInvTimes(testData.getInvTimesPtr());
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

TEST(OxSignCalculatorShmolli, calculateSign_throwIfInvTimesNotSet) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};
    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    // signCalculator.setInvTimes(testData.getInvTimesPtr());
    signCalculator.setNSamples(testData.getNSamples());
    signCalculator.setSigMag(testData.getSignalMagPtr());
    signCalculator.setSigPha(testData.getSignalPhaPtr());
    signCalculator.setSignal(signal);
    signCalculator.setSigns(signs);

    EXPECT_THROW(signCalculator.calculateSign(), std::runtime_error);

    delete [] signal;
    delete [] signs;
}

TEST(OxSignCalculatorShmolli, copConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};
    TYPE *signal = new TYPE[nSamples];
    TYPE *signs = new TYPE[nSamples];

    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    signCalculator.setInvTimes(testData.getInvTimesPtr());
    signCalculator.setNSamples(testData.getNSamples());
    signCalculator.setSigMag(testData.getSignalMagPtr());
    signCalculator.setSigPha(testData.getSignalPhaPtr());
    signCalculator.setSignal(signal);
    signCalculator.setSigns(signs);

    Ox::SignCalculatorShmolli<TYPE> signCalculatorCopy(signCalculator);

    // same nSamples
    EXPECT_EQ(signCalculator.getNSamples(), signCalculatorCopy.getNSamples());

    // empty pointers
    EXPECT_THROW(signCalculatorCopy.getInvTimes(), std::runtime_error);
    EXPECT_THROW(signCalculatorCopy.getSigMag(), std::runtime_error);
    EXPECT_THROW(signCalculatorCopy.getSigPha(), std::runtime_error);
    EXPECT_THROW(signCalculatorCopy.getSignal(), std::runtime_error);
    EXPECT_THROW(signCalculatorCopy.getSigns(), std::runtime_error);

    delete [] signal;
    delete [] signs;
}

#endif // USE_PRIVATE_NR2
