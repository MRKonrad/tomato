/*!
 * \file OxModelT2TwoParamScale_test.cpp
 * \author Konrad Werys
 * \date 2020/11/19
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2
#include "OxModelT2TwoParamScale.h"

TEST(OxModelT2TwoParamScale, calcModelValueNoSignalTest) {

    typedef double TYPE;

    TYPE params[] = {5, 100};

    Ox::ModelT2TwoParamScale<TYPE> model;
    
    EXPECT_THROW(model.calcModelValue(params, 0), std::runtime_error);
}

TEST(OxModelT2TwoParamScale, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[] = {5, 100};
    TYPE signal[] = {100, 40, 30, 10};
    int nSamples = 4;

    Ox::ModelT2TwoParamScale<TYPE> model;
    model.setSignal(signal);
    model.setNSamples(nSamples);

    EXPECT_DOUBLE_EQ(model.calcModelValue(params, 0), 5);
}

TEST(OxModelT2TwoParamScale, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::ModelT2TwoParamScale<TYPE> model;
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

TEST(OxModelT2TwoParamScale, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {100, 130};

    Ox::ModelT2TwoParamScale<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11*2];

    model.calcLSJacobian(params, jacobian);

    TYPE correct[] = {
            1, 0,
            0.680712, 0.201394,
            0.463369, 0.274183,
            0.315421, 0.27996,
            0.214711, 0.254096,
            0.146157, 0.216208,
            0.0994906, 0.17661,
            0.0677245, 0.140258,
            0.0461009, 0.109115,
            0.0313814, 0.0835601,
            0.0213617, 0.0632004
    };

    for (int iSample = 0; iSample < nSamples; iSample++) {
        for (int iDim = 0; iDim < 2; iDim++) {
            EXPECT_NEAR(jacobian[iSample*2+iDim], correct[iSample*2+iDim], 1e-3);
        }
    }
}

TEST(OxModelT2TwoParamScale, calcCostValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::ModelT2TwoParamScale<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(model.calcCostValue(params), 18630.38);
}

TEST(OxModelT2TwoParamScale, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {5, 100};

    Ox::ModelT2TwoParamScale<TYPE> model;
    model.setNSamples(nSamples);
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0};
    model.calcCostDerivative(params, derivative);

    EXPECT_NEAR(derivative[0], -324.80121329761283, 1e-3);
    EXPECT_NEAR(derivative[1], -5.7517724315963772, 1e-3);

}

TEST(OxModelT2TwoParamScale, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE signal[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal[11] = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    TYPE newSignal2[11] = {3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    model.setNSamples(testData.getNSamples());
    model.setEchoTimes(testData.getEchoTimesPtr());
    model.setSignal(signal);

    // copy and set signal
    Ox::ModelT2TwoParamScale<TYPE> modelCopy = model;
    modelCopy.setSignal(newSignal);

    // copy and set signal
    Ox::ModelT2TwoParamScale<TYPE> modelCopy2(model);
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

#endif // USE_PRIVATE_NR2