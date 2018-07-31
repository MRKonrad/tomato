/*!
 * \file OxFunctionsT1Basic_test.cpp
 * \author Konrad Werys
 * \date 2018/08/29
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFunctionsT1Basic.h"

TEST(OxShmolli2, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[3] = {100, 200, 1000};

    Ox::FunctionsT1Basic<TYPE> functionsObject(0);
    functionsObject.setParameters(params);

    EXPECT_DOUBLE_EQ(functionsObject.calcModelValue(0), -100);
}

TEST(OxShmolli2, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::FunctionsT1Basic<TYPE> functionsObject(nSamples);
    functionsObject.setParameters(params);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE *residuals = new TYPE[nSamples];
    functionsObject.calcLSResiduals(residuals);

    for (int i = 0; i < nSamples; i++){
        EXPECT_DOUBLE_EQ(residuals[i], -testData.getSignalMag()[i]);
    }

    delete [] residuals;
}

TEST(OxShmolli2, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject(nSamples);
    functionsObject.setParameters(params);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[7*3];

    functionsObject.calcLSJacobian(jacobian);

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

TEST(OxShmolli2, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::FunctionsT1Basic<TYPE> functionsObject(nSamples);
    functionsObject.setParameters(params);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(functionsObject.calcCostValue(), 17169);
}

TEST(OxShmolli2, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject(nSamples);
    functionsObject.setParameters(params);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0, 0};
    functionsObject.calcCostDerivative(derivative);

    EXPECT_NEAR(derivative[0], -425.52433072265057, 1e-3);
    EXPECT_NEAR(derivative[1],  588.84996553808264, 1e-3);
    EXPECT_NEAR(derivative[2],    7.36064616633549, 1e-3);

}

