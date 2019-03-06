/*!
 * \file OxFunctionsT1Shmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/07/29
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"
#include "OxTestData.h"

#ifdef USE_PRIVATE_NR2
#include "OxFunctionsT1Shmolli.h"

TEST(OxFunctionsT1Shmolli, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[3] = {100, 200, 1000};

    Ox::FunctionsT1Shmolli<TYPE> functionsObject;

    EXPECT_DOUBLE_EQ(functionsObject.calcModelValue(params, 0), 100); // because shmolli
}

TEST(OxFunctionsT1Shmolli, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::FunctionsT1Shmolli<TYPE> functionsObject;
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

TEST(OxFunctionsT1Shmolli, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 1200};

    Ox::FunctionsT1Shmolli<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[7*3];

    functionsObject.calcLSJacobian(params, jacobian);

    TYPE correct[7*3] = {
            1, -0.920044,   0,
            1, -0.860708,   0,
            1, -0.805198,   0,
            1, -0.239508,   0,
            1, -0.0619868,  0,
            1, -0.0167532,  0,
            1, -0.00461166, 0,
    };

    for (int iSample = 0; iSample < nSamples; iSample++) {
        for (int iDim = 0; iDim < 3; iDim++) {
            EXPECT_NEAR(jacobian[iSample*3+iDim], correct[iSample*3+iDim], 1e-3);
        }
    }
}

TEST(OxFunctionsT1Shmolli, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::FunctionsT1Shmolli<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(functionsObject.calcCostValue(params), 17169);
}

TEST(OxFunctionsT1Shmolli, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::FunctionsT1Shmolli<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0, 0};
    functionsObject.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -425.52433072265057, 1e-3);
    EXPECT_NEAR(derivative[1],  588.84996553808264, 1e-3);
    EXPECT_NEAR(derivative[2],    7.36064616633549, 1e-3);

}

TEST(OxFunctionsT1Shmolli, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[7] = {1, 2, 3, 4, 5, 6, 7};
    TYPE newSignal[7] = {2, 2, 3, 4, 5, 6, 7};
    TYPE newSignal2[7] = {3, 2, 3, 4, 5, 6, 7};

    // init the necessary objects
    Ox::FunctionsT1Shmolli<TYPE> functionsObject;
    functionsObject.setNSamples(testData.getNSamples());
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(signal);

    // copy and set signal
    Ox::FunctionsT1Shmolli<TYPE> functionsObjectCopy = functionsObject;
    functionsObjectCopy.setSignal(newSignal);

    // copy and set signal
    Ox::FunctionsT1Shmolli<TYPE> functionsObjectCopy2(functionsObject);
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

#endif // USE_PRIVATE_NR2