/*!
 * \file OxCalculatorT2_test.cpp
 * \author Konrad Werys
 * \date 2019/11/01
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2
#ifdef USE_YAML

#include "OxModelT2TwoParam.h"
#include "OxModelT2ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT2.h"

#ifdef USE_VNL
TEST(OxCalculatorT2, blood) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2<TYPE> calculatorT2;

    // configure
    TYPE startPoint[] = {1, 100, 50};
    startPointCalculator.setInputStartPoint(startPoint);
    calculatorT2.setStartPointCalculator(&startPointCalculator);
    calculatorT2.setModel(&model);
    calculatorT2.setFitter(&fitter);

    // set the data
    calculatorT2.setNSamples(nSamples);
    calculatorT2.setEchoTimes(testData.getEchoTimesPtr());
    calculatorT2.setSigMag(testData.getSignalMagPtr());

    calculatorT2.calculate();

    EXPECT_NEAR(calculatorT2.getResults()["A"],  testData.getResultsThreeParam()[0], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["B"],  testData.getResultsThreeParam()[1], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["T2"], testData.getResultsThreeParam()[2], 1e-1);
}

TEST(OxCalculatorT2, myo) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2<TYPE> calculatorT2;

    // configure
    TYPE startPoint[] = {1, 100, 50};
    startPointCalculator.setInputStartPoint(startPoint);
    calculatorT2.setStartPointCalculator(&startPointCalculator);
    calculatorT2.setModel(&model);
    calculatorT2.setFitter(&fitter);


    // set the data
    calculatorT2.setNSamples(nSamples);
    calculatorT2.setEchoTimes(testData.getEchoTimesPtr());
    calculatorT2.setSigMag(testData.getSignalMagPtr());

    calculatorT2.calculate();

    EXPECT_NEAR(calculatorT2.getResults()["A"],  testData.getResultsThreeParam()[0], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["B"],  testData.getResultsThreeParam()[1], 1e-1);
    EXPECT_NEAR(calculatorT2.getResults()["T2"], testData.getResultsThreeParam()[2], 1e-1);
}

TEST(OxCalculatorT2, myo_3samples) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2<TYPE> calculatorT2;

    // configure
    TYPE startPoint[] = { 1, 80 };
    startPointCalculator.setInputStartPoint(startPoint);
    calculatorT2.setStartPointCalculator(&startPointCalculator);
    calculatorT2.setModel(&model);
    calculatorT2.setFitter(&fitter);

    // set the data
    calculatorT2.setNSamples(nSamples);
    calculatorT2.setEchoTimes(testData.getEchoTimesPtr());
    calculatorT2.setSigMag(testData.getSignalMagPtr());

    calculatorT2.calculate();

    EXPECT_NEAR(calculatorT2.getResults()["A"],  testData.getResultsTwoParam()[0], 2e-1);
    EXPECT_NEAR(calculatorT2.getResults()["T2"], testData.getResultsTwoParam()[1], 2e-1);
}

TEST(OxCalculatorT2, myocardiumTwoParam) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2<TYPE> calculatorT2;

    // configure
    calculatorT2.setModel(&model);
    calculatorT2.setFitter(&fitter);

    // set the data
    calculatorT2.setNSamples(nSamples);
    calculatorT2.setEchoTimes(testData.getEchoTimesPtr());
    calculatorT2.setSigMag(testData.getSignalMagPtr());

    calculatorT2.calculate();

    EXPECT_NEAR(calculatorT2.getResults()["deltaA"], 0.02507, 1e-4);
    EXPECT_NEAR(calculatorT2.getResults()["deltaT2"], 0.02765, 1e-4);
}

TEST(OxCalculatorT2, myocardiumThreeParam) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2<TYPE> calculatorT2;

    // configure
    calculatorT2.setModel(&model);
    calculatorT2.setFitter(&fitter);

    // set the data
    calculatorT2.setNSamples(nSamples);
    calculatorT2.setEchoTimes(testData.getEchoTimesPtr());
    calculatorT2.setSigMag(testData.getSignalMagPtr());

    calculatorT2.calculate();

    EXPECT_NEAR(calculatorT2.getResults()["deltaA"], 0.01036, 1e-4);
    EXPECT_NEAR(calculatorT2.getResults()["deltaB"], 0.02802, 1e-4);
    EXPECT_NEAR(calculatorT2.getResults()["deltaT2"], 0.03368, 1e-4);
}

TEST(OxCalculatorT2, calculateFitError) {

    typedef double TYPE;

    typedef double TYPE;

    char filePath [] = "testData/T2_blood_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double tolerance = 1e-4;

    double correctDeltaA = 0.1515;
    double correctDeltaT2 = 0.38496;

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
}

TEST(OxCalculatorT2, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitterAmoebaVnl);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.setMeanCutOff(123);

    Ox::CalculatorT2<TYPE> calculatorCopy = calculator;

    EXPECT_EQ(calculator.getMeanCutOff(), calculatorCopy.getMeanCutOff());
    EXPECT_EQ(calculator.getNSamples(), calculatorCopy.getNSamples());
    EXPECT_EQ(calculator.getNDims(), calculatorCopy.getNDims());

    // empty object pointers
    EXPECT_THROW(calculatorCopy.getModel(), std::runtime_error);
    EXPECT_THROW(calculatorCopy.getFitter(), std::runtime_error);

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
#endif // USE_YAML
#endif // USE_PRIVATE_NR2