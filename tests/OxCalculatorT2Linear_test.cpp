/*!
 * \file OxCalculatorT2Linear_test.cpp
 * \author Konrad Werys
 * \date 2019/11/01
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2

#include "OxModelT2TwoParam.h"
#include "OxModelT2ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT2Linear.h"

#ifdef USE_VNL
TEST(OxCalculatorT2Linear, blood) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    Ox::CalculatorT2Linear<TYPE> calculator;

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], testData.getResultsThreeParam()[1], 1);
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsThreeParam()[2], 1);
}

TEST(OxCalculatorT2Linear, myo) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    Ox::CalculatorT2Linear<TYPE> calculator;

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], testData.getResultsThreeParam()[1], 10); // !!!
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsThreeParam()[2], 1);
}

TEST(OxCalculatorT2Linear, myo_3samples) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    Ox::CalculatorT2Linear<TYPE> calculator;

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], testData.getResultsThreeParam()[1], 2e-1);
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsThreeParam()[2], 2e-1);
}

TEST(OxCalculatorT2Linear, blood_3samples) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    Ox::CalculatorT2Linear<TYPE> calculator;

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], testData.getResultsThreeParam()[1], 2e-1);
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsThreeParam()[2], 2e-1);
}


TEST(OxCalculatorT2Linear, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    //Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2Linear<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitterAmoebaVnl);
    //calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.setMeanCutOff(123);

    Ox::CalculatorT2Linear<TYPE> calculatorCopy = calculator;

    EXPECT_EQ(calculator.getMeanCutOff(), calculatorCopy.getMeanCutOff());
    EXPECT_EQ(calculator.getNSamples(), calculatorCopy.getNSamples());
    EXPECT_EQ(calculator.getNDims(), calculatorCopy.getNDims());

    // empty array pointers
    EXPECT_THROW(calculatorCopy.getEchoTimes(), std::runtime_error);
    EXPECT_FALSE(calculatorCopy.getRepTimes());
    EXPECT_FALSE(calculatorCopy.getRelAcqTimes());
    EXPECT_THROW(calculatorCopy.getSigMag(), std::runtime_error);
    EXPECT_FALSE(calculatorCopy.getSigPha());

    // non-empty pointers of internal arrays
    EXPECT_TRUE(calculatorCopy.getSignal());
    EXPECT_TRUE(calculatorCopy.getSigns());
    EXPECT_TRUE(calculatorCopy.getStartPoint());

}

#endif // USE_VNL
#endif // USE_PRIVATE_NR2