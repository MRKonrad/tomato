/*!
 * \file OxModelT2TwoParam_test.cpp
 * \author Konrad Werys
 * \date 2019/12/03
 */

#include "CmakeConfigForTomato.h"
#include "gtest/gtest.h"

#ifdef USE_YAML
#include "OxTestData.h"

#ifdef USE_PRIVATE_NR2
#include "OxModelT2TwoParam.h"

TEST(OxModelT2TwoParam, calcModelValueNoSignalTest) {

    typedef double TYPE;

    TYPE params[] = {5, 100};

    Ox::ModelT2TwoParam<TYPE> model;

    EXPECT_THROW(model.calcModelValue(params, 0), std::runtime_error);
}

TEST(OxModelT2TwoParam, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[] = {5, 100};
    TYPE signal[] = {100, 40, 30, 10};
    int nSamples = 4;

    Ox::ModelT2TwoParam<TYPE> model;
    model.setSignal(signal);
    model.setNSamples(nSamples);

    EXPECT_DOUBLE_EQ(model.calcModelValue(params, 0), 105);
}

TEST(OxModelT2TwoParam, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::ModelT2TwoParam<TYPE> model;
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

TEST(OxModelT2TwoParam, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 50};

    Ox::ModelT2TwoParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11*2];

    model.calcLSJacobian(params, jacobian);

    TYPE correct[] = {
            1,           0,
            0.367879,    0.735759,
            0.135335,    0.541341,
            0.0497871,   0.298722,
            0.0183156,   0.146525,
            0.00673795,  0.0673795,
            0.00247875,  0.029745,
            0.000911882, 0.0127663,
            0.000335463, 0.0053674,
            0.00012341,  0.00222138,
            4.53999e-05, 0.000907999
    };

    for (int iSample = 0; iSample < nSamples; iSample++) {
        for (int iDim = 0; iDim < 2; iDim++) {
            EXPECT_NEAR(jacobian[iSample*2+iDim], correct[iSample*2+iDim], 1e-3);
        }
    }
}

TEST(OxModelT2TwoParam, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::ModelT2TwoParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(model.calcCostValue(params), 18630.38);
}

TEST(OxModelT2TwoParam, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {5, 100};

    Ox::ModelT2TwoParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0};
    model.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -0.97848457814767542, 1e-3);
    EXPECT_NEAR(derivative[1],  -8.6633496944149666, 1e-3);

}

TEST(OxModelT2TwoParam, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal[11] = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal2[11] = {3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> model;
    model.setNSamples(testData.getNSamples());
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(signal);

    // copy and set signal
    Ox::ModelT2TwoParam<TYPE> modelCopy = model;
    modelCopy.setSignal(newSignal);

    // copy and set signal
    Ox::ModelT2TwoParam<TYPE> modelCopy2(model);
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