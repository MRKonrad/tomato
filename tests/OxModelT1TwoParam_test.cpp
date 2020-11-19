/*!
 * \file OxModelT1TwoParam_test.cpp
 * \author Konrad Werys
 * \date 2018/07/29
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxModelT1TwoParam.h"

TEST(OxModelT1ThParams, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[] = {100, 1000};
    TYPE time = 500;
    Ox::ModelT1TwoParam<TYPE> model;
    //model.setParameters(params);

    EXPECT_DOUBLE_EQ(model.calcModelValue(params, time), params[0]*(1 - exp(-time/params[1])) );
}

TEST(OxModelT1TwoParam, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::ModelT1TwoParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE *residuals = new TYPE[nSamples];
    model.calcLSResiduals(params, residuals);

    for (int i = 0; i < nSamples; i++){
        EXPECT_DOUBLE_EQ(residuals[i], -testData.getSignalMag()[i]);
    }

    delete [] residuals;
}

TEST(OxModelT1TwoParam, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {10, 1000};

    Ox::ModelT1TwoParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11*2];

    model.calcLSJacobian(params, jacobian);

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

TEST(OxModelT1TwoParam, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::ModelT1TwoParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(model.calcCostValue(params), 510504);
}

TEST(OxModelT1TwoParam, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {100, 1200};

    Ox::ModelT1TwoParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0};
    model.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -1554.171, 1e-3);
    EXPECT_NEAR(derivative[1],  36.812, 1e-3);

}

TEST(OxModelT1TwoParam, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T1_saturation_recovery.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // init the necessary objects
    Ox::ModelT1TwoParam<TYPE> model;
    model.setNSamples(testData.getNSamples());
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(signal);

    // copy and set signal
    Ox::ModelT1TwoParam<TYPE> modelCopy = model;
    modelCopy.setSignal(newSignal);

    // copy and set signal
    Ox::ModelT1TwoParam<TYPE> modelCopy2(model);
    modelCopy2.setSignal(newSignal2);

    // copy should preserve nSamples
    EXPECT_EQ(modelCopy.getNSamples(), nSamples);
    EXPECT_EQ(modelCopy2.getNSamples(), nSamples);

    // copy should reset invTimes pointer
    EXPECT_FALSE(modelCopy.getInvTimes());
    EXPECT_FALSE(modelCopy2.getInvTimes());

    // copy should not preserve signal pointer
    EXPECT_NE(model.getSignal(), modelCopy.getSignal());
    EXPECT_NE(model.getSignal(), modelCopy2.getSignal());

    // check if the new signal pointers were asigned
    EXPECT_EQ(modelCopy.getSignal(), newSignal);
    EXPECT_EQ(modelCopy2.getSignal(), newSignal2);

}

