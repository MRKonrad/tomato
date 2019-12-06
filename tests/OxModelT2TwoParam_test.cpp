/*!
 * \file OxModelT2TwoParam_test.cpp
 * \author Konrad Werys
 * \date 2019/12/03
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2
#include "OxModelT2TwoParam.h"

TEST(OxModelT2TwoParam, calcModelValueNoSignalTest) {

    typedef double TYPE;

    TYPE params[] = {5, 100};

    Ox::ModelT2TwoParam<TYPE> functionsObject;

    EXPECT_THROW(functionsObject.calcModelValue(params, 0), std::runtime_error);
}

TEST(OxModelT2TwoParam, calcModelValueTest) {

    typedef double TYPE;

    TYPE params[] = {5, 100};
    TYPE signal[] = {100, 40, 30, 10};
    int nSamples = 4;

    Ox::ModelT2TwoParam<TYPE> functionsObject;
    functionsObject.setSignal(signal);
    functionsObject.setNSamples(nSamples);

    EXPECT_DOUBLE_EQ(functionsObject.calcModelValue(params, 0), 105);
}

TEST(OxModelT2TwoParam, calcLSResidualsTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 0};

    Ox::ModelT2TwoParam<TYPE> functionsObject;
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

TEST(OxModelT2TwoParam, calcLSJacobianTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {0, 50};

    Ox::ModelT2TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE jacobian[11*2];

    functionsObject.calcLSJacobian(params, jacobian);

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

    Ox::ModelT2TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    EXPECT_DOUBLE_EQ(functionsObject.calcCostValue(params), 18630.38);
}

TEST(OxModelT2TwoParam, calcCostDerivativeTest) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[] = {5, 100};

    Ox::ModelT2TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(testData.getSignalMagPtr());

    TYPE derivative[] = {0, 0};
    functionsObject.calcCostDerivative(params, derivative);

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
    Ox::ModelT2TwoParam<TYPE> functionsObject;
    functionsObject.setNSamples(testData.getNSamples());
    functionsObject.setEchoTimes(testData.getEchoTimesPtr());
    functionsObject.setSignal(signal);

    // copy and set signal
    Ox::ModelT2TwoParam<TYPE> functionsObjectCopy = functionsObject;
    functionsObjectCopy.setSignal(newSignal);

    // copy and set signal
    Ox::ModelT2TwoParam<TYPE> functionsObjectCopy2(functionsObject);
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