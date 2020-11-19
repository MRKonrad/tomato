/*!
 * \file OxModelT2OneParam_test.cpp
 * \author Konrad Werys
 * \date 2019/12/03
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2
#include "OxModelT2OneParam.h"

TEST(OxModelT2OneParam, calcModelValueNoSignalTest) {

    typedef double TYPE;

    TYPE params[] = {100};

    Ox::ModelT2OneParam<TYPE> functionsObject;

    EXPECT_THROW(functionsObject.calcModelValue(params, 0), std::runtime_error);
}

TEST(OxModelT2OneParam, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[] = {100};
    TYPE signal[] = {100, 40, 30, 10};
    int nSamples = 4;

    Ox::ModelT2OneParam<TYPE> functionsObject;
    functionsObject.setSignal(signal);
    functionsObject.setNSamples(nSamples);

    EXPECT_DOUBLE_EQ(functionsObject.calcModelValue(params, 0), 100);
}

TEST(OxModelT2OneParam, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0};

    Ox::ModelT2OneParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE *residuals = new TYPE[nSamples];
    functionsObject.calcLSResiduals(params, residuals);

    for (int i = 0; i < nSamples; i++){
        EXPECT_DOUBLE_EQ(residuals[i], -testData.getSignalMag()[i]);
    }

    delete [] residuals;
}

TEST(OxModelT2OneParam, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {150};

    Ox::ModelT2OneParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11];

    functionsObject.calcLSJacobian(params, jacobian);

    TYPE correct[] = {0, 0.159229, 0.228185, 0.245253, 0.234309, 0.209862, 0.180447, 0.150845, 0.123526, 0.0995741, 0.0792755};

    for (int iSample = 0; iSample < nSamples; iSample++) {
        EXPECT_NEAR(jacobian[iSample], correct[iSample], 1e-3);
    }
}

TEST(OxModelT2OneParam, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0};

    Ox::ModelT2OneParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(functionsObject.calcCostValue(params), 18630.38);
}

TEST(OxModelT2OneParam, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {100};

    Ox::ModelT2OneParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0};
    functionsObject.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -27.600528657790338, 1e-3);

}

TEST(OxModelT2OneParam, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal[11] = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal2[11] = {3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // init the necessary objects
    Ox::ModelT2OneParam<TYPE> functionsObject;
    functionsObject.setNSamples(testData.getNSamples());
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(signal);

    // copy and set signal
    Ox::ModelT2OneParam<TYPE> functionsObjectCopy = functionsObject;
    functionsObjectCopy.setSignal(newSignal);

    // copy and set signal
    Ox::ModelT2OneParam<TYPE> functionsObjectCopy2(functionsObject);
    functionsObjectCopy2.setSignal(newSignal2);

    // copy should preserve nSamples
    EXPECT_EQ(functionsObjectCopy.getNSamples(), nSamples);
    EXPECT_EQ(functionsObjectCopy2.getNSamples(), nSamples);

    // copy should reset echoTimes pointer
    EXPECT_FALSE(functionsObjectCopy.getEchoTimes());
    EXPECT_FALSE(functionsObjectCopy2.getEchoTimes());

    // copy should not preserve signal pointer
    EXPECT_NE(functionsObject.getSignal(), functionsObjectCopy.getSignal());
    EXPECT_NE(functionsObject.getSignal(), functionsObjectCopy2.getSignal());

    // check if the new signal pointers were asigned
    EXPECT_EQ(functionsObjectCopy.getSignal(), newSignal);
    EXPECT_EQ(functionsObjectCopy2.getSignal(), newSignal2);

}

#endif // USE_PRIVATE_NR2