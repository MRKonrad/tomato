/*!
 * \file OxModelT1Shmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/07/29
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"
#include "OxTestData.h"

#ifdef USE_PRIVATE_NR2
#include "OxModelT1Shmolli.h"

TEST(OxModelT1Shmolli, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[3] = {100, 200, 1000};

    Ox::ModelT1Shmolli<TYPE> model;

    EXPECT_DOUBLE_EQ(model.calcModelValue(params, 0), 100); // because shmolli
}

TEST(OxModelT1Shmolli, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::ModelT1Shmolli<TYPE> model;
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

TEST(OxModelT1Shmolli, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 1200};

    Ox::ModelT1Shmolli<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[7*3];

    model.calcLSJacobian(params, jacobian);

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

TEST(OxModelT1Shmolli, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::ModelT1Shmolli<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(model.calcCostValue(params), 17169);
}

TEST(OxModelT1Shmolli, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::ModelT1Shmolli<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0, 0};
    model.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -425.52433072265057, 1e-3);
    EXPECT_NEAR(derivative[1],  588.84996553808264, 1e-3);
    EXPECT_NEAR(derivative[2],    7.36064616633549, 1e-3);

}

TEST(OxModelT1Shmolli, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[7] = {1, 2, 3, 4, 5, 6, 7};
    TYPE newSignal[7] = {2, 2, 3, 4, 5, 6, 7};
    TYPE newSignal2[7] = {3, 2, 3, 4, 5, 6, 7};

    // init the necessary objects
    Ox::ModelT1Shmolli<TYPE> model;
    model.setNSamples(testData.getNSamples());
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(signal);

    // copy and set signal
    Ox::ModelT1Shmolli<TYPE> modelCopy = model;
    modelCopy.setSignal(newSignal);

    // copy and set signal
    Ox::ModelT1Shmolli<TYPE> modelCopy2(model);
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

#endif // USE_PRIVATE_NR2