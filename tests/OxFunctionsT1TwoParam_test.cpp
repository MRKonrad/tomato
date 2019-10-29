/*!
 * \file OxFunctionsT1TwoParams_test.cpp
 * \author Konrad Werys
 * \date 2018/07/29
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFunctionsT1TwoParam.h"

TEST(OxFunctionsT1ThParams, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[] = {100, 1000};
    TYPE time = 500;
    Ox::FunctionsT1TwoParam<TYPE> functionsObject;
    //functionsObject.setParameters(params);

    EXPECT_DOUBLE_EQ(functionsObject.calcModelValue(params, time), params[0]*(1 - exp(-time/params[1])) );
}

TEST(OxFunctionsT1TwoParams, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::FunctionsT1TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE *residuals = new TYPE[nSamples];
    functionsObject.calcLSResiduals(params, residuals);

    for (int i = 0; i < nSamples; i++){
        EXPECT_DOUBLE_EQ(residuals[i], -testData.getSignalMag()[i]);
    }

    delete [] residuals;
}

TEST(OxFunctionsT1TwoParams, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {10, 1000};

    Ox::FunctionsT1TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11*2];

    functionsObject.calcLSJacobian(params, jacobian);

    TYPE correct[] = {
            0.0796489, -0.000763891,
            0.14187  , -0.00131294,
            0.199885 , -0.00178426,
            0.253978 , -0.00218584,
            0.304414 , -0.00252498,
            0.35144  , -0.00280827,
            0.395286 , -0.00304171,
            0.436169 , -0.00323075,
            0.474287 , -0.00338033,
            0.509829 , -0.00349492,
            0.999955 , -4.53999e-06
    };

    for (int iSample = 0; iSample < nSamples; iSample++) {
        for (int iDim = 0; iDim < 2; iDim++) {
            EXPECT_NEAR(jacobian[iSample*2+iDim], correct[iSample*2+iDim], 1e-6);
        }
    }
}

TEST(OxFunctionsT1TwoParams, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::FunctionsT1TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(functionsObject.calcCostValue(params), 510504);
}

TEST(OxFunctionsT1TwoParams, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {100, 1200};

    Ox::FunctionsT1TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0};
    functionsObject.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -1554.171, 1e-3);
    EXPECT_NEAR(derivative[1],  36.812, 1e-3);

}

TEST(OxFunctionsT1TwoParams, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // init the necessary objects
    Ox::FunctionsT1TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(testData.getNSamples());
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(signal);

    // copy and set signal
    Ox::FunctionsT1TwoParam<TYPE> functionsObjectCopy = functionsObject;
    functionsObjectCopy.setSignal(newSignal);

    // copy and set signal
    Ox::FunctionsT1TwoParam<TYPE> functionsObjectCopy2(functionsObject);
    functionsObjectCopy2.setSignal(newSignal2);

    // copy should preserve nSamples
    EXPECT_EQ(functionsObjectCopy.getNSamples(), nSamples);
    EXPECT_EQ(functionsObjectCopy2.getNSamples(), nSamples);

    // copy should reset invTimes pointer
    EXPECT_FALSE(functionsObjectCopy.getInvTimes());
    EXPECT_FALSE(functionsObjectCopy2.getInvTimes());

    // copy should not preserve signal pointer
    EXPECT_NE(functionsObject.getSignal(), functionsObjectCopy.getSignal());
    EXPECT_NE(functionsObject.getSignal(), functionsObjectCopy2.getSignal());

    // check if the new signal pointers were asigned
    EXPECT_EQ(functionsObjectCopy.getSignal(), newSignal);
    EXPECT_EQ(functionsObjectCopy2.getSignal(), newSignal2);

}

