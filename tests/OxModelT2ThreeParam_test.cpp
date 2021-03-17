/*!
 * \file OxModelT2ThreeParam_test.cpp
 * \author Konrad Werys
 * \date 2018/07/29
 */

#include "CmakeConfigForTomato.h"
#include "gtest/gtest.h"

#ifdef USE_YAML
#include "OxTestData.h"

#ifdef USE_PRIVATE_NR2
#include "OxModelT2ThreeParam.h"

TEST(OxModelT2ThreeParam, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[3] = {5, 100, 50};

    Ox::ModelT2ThreeParam<TYPE> model;

    EXPECT_DOUBLE_EQ(model.calcModelValue(params, 0), 105);
}

TEST(OxModelT2ThreeParam, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::ModelT2ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE *residuals = new TYPE[nSamples];
    model.calcLSResiduals(params, residuals);

    for (int i = 0; i < nSamples; i++){
        EXPECT_DOUBLE_EQ(residuals[i], -testData.getSignalMag()[i]);
    }

    delete [] residuals;
}

TEST(OxModelT2ThreeParam, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 50};

    Ox::ModelT2ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11*3];

    model.calcLSJacobian(params, jacobian);

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

TEST(OxModelT2ThreeParam, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};

    Ox::ModelT2ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(model.calcCostValue(params), 18630.38);
}

TEST(OxModelT2ThreeParam, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {5, 100, 50};

    Ox::ModelT2ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0, 0};
    model.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -190.8099429664577, 1e-3);
    EXPECT_NEAR(derivative[1],  -19.7011203248681, 1e-3);
    EXPECT_NEAR(derivative[2],  -87.4480936756914, 1e-3);

}

TEST(OxModelT2ThreeParam, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal[11] = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal2[11] = {3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> model;
    model.setNSamples(testData.getNSamples());
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(signal);

    // copy and set signal
    Ox::ModelT2ThreeParam<TYPE> modelCopy = model;
    modelCopy.setSignal(newSignal);

    // copy and set signal
    Ox::ModelT2ThreeParam<TYPE> modelCopy2(model);
    modelCopy2.setSignal(newSignal2);

    // copy should preserve nSamples
    EXPECT_EQ(modelCopy.getNSamples(), nSamples);
    EXPECT_EQ(modelCopy2.getNSamples(), nSamples);

    // copy should reset echoTimes pointer
    EXPECT_FALSE(modelCopy.getEchoTimes());
    EXPECT_FALSE(modelCopy2.getEchoTimes());

    // copy should not preserve signal pointer
    EXPECT_NE(model.getSignal(), modelCopy.getSignal());
    EXPECT_NE(model.getSignal(), modelCopy2.getSignal());

    // check if the new signal pointers were asigned
    EXPECT_EQ(modelCopy.getSignal(), newSignal);
    EXPECT_EQ(modelCopy2.getSignal(), newSignal2);

}

#endif // USE_YAML
#endif // USE_PRIVATE_NR2