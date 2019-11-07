/*!
 * \file OxFunctionsT2ThreeParam_test.cpp
 * \author Konrad Werys
 * \date 2018/07/29
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFunctionsT2ThreeParam.h"

TEST(OxFunctionsT2ThreeParam, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[3] = {5, 100, 50};

    Ox::FunctionsT2ThreeParam<TYPE> functionsObject;

    EXPECT_DOUBLE_EQ(functionsObject.calcModelValue(params, 0), 105);
}

TEST(OxFunctionsT2ThreeParam, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::FunctionsT2ThreeParam<TYPE> functionsObject;
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

TEST(OxFunctionsT2ThreeParam, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 50};

    Ox::FunctionsT2ThreeParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11*3];

    functionsObject.calcLSJacobian(params, jacobian);

    TYPE correct[11*3] = {
            1,        1,    0,
            1, 0.367879,    0,
            1, 0.135335,    0,
            1, 0.0497871,   0,
            1, 0.0183156,   0,
            1, 0.00673795,  0,
            1, 0.00247875,  0,
            1, 0.000911882, 0,
            1, 0.000335463, 0,
            1, 0.00012341,  0,
            1, 4.53999e-05, 0,
    };

    for (int iSample = 0; iSample < nSamples; iSample++) {
        for (int iDim = 0; iDim < 3; iDim++) {
            EXPECT_NEAR(jacobian[iSample*3+iDim], correct[iSample*3+iDim], 1e-3);
        }
    }
}

TEST(OxFunctionsT2ThreeParam, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::FunctionsT2ThreeParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(functionsObject.calcCostValue(params), 18630.38);
}

TEST(OxFunctionsT2ThreeParam, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {5, 100, 50};

    Ox::FunctionsT2ThreeParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0, 0};
    functionsObject.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -190.8099429664577, 1e-3);
    EXPECT_NEAR(derivative[1],  -19.7011203248681, 1e-3);
    EXPECT_NEAR(derivative[2],  -87.4480936756914, 1e-3);

}

TEST(OxFunctionsT2ThreeParam, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal[11] = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal2[11] = {3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // init the necessary objects
    Ox::FunctionsT2ThreeParam<TYPE> functionsObject;
    functionsObject.setNSamples(testData.getNSamples());
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(signal);

    // copy and set signal
    Ox::FunctionsT2ThreeParam<TYPE> functionsObjectCopy = functionsObject;
    functionsObjectCopy.setSignal(newSignal);

    // copy and set signal
    Ox::FunctionsT2ThreeParam<TYPE> functionsObjectCopy2(functionsObject);
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

