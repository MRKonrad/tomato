/*!
 * \file OxCalculatorT2Truncation_test.cpp
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
#include "OxCalculatorT2Truncation.h"

#ifdef USE_VNL
TEST(OxCalculatorT2Truncation, blood) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();
    TYPE noise[] = {2.3, 1.2, 3.2, 1.1, 4.3, 2.3, 1.2, 3.2, 1.1, 4.3, 3.4};
    //TYPE startPoint[] = {1, 100, 50};

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    //calculator.setStartPointCalculator(&startPointCalculator);
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());
    calculator.setNoise(noise);
    //startPointCalculator.setInputStartPoint(startPoint);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], testData.getResultsThreeParam()[0], 2e-1);
    EXPECT_NEAR(calculator.getResults()["B"], testData.getResultsThreeParam()[1], 2e-1);
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsThreeParam()[2], 2e-1);
}

TEST(OxCalculatorT2Truncation, myo) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();
    TYPE noise[] = {2.3, 1.2, 3.2, 1.1, 4.3, 2.3, 1.2, 3.2, 1.1, 4.3, 3.4};
    //TYPE startPoint[] = {1, 100, 50};

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> functionsObject;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    //Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    //calculator.setStartPointCalculator(&startPointCalculator);
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());
    calculator.setNoise(noise);
    //startPointCalculator.setInputStartPoint(startPoint);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], testData.getResultsTwoParam()[0], 2e-1);
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsTwoParam()[1], 2e-1);
}

TEST(OxCalculatorT2Truncation, myo_3samples) {

    typedef double TYPE;

    char filePath [] = "testData/T2_myocardium_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();
    TYPE noise[] = {2.3, 1.2, 3.2, 1.1, 4.3, 2.3, 1.2, 3.2, 1.1, 4.3, 3.4};
    //TYPE startPoint[] = {500, 80};

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> functionsObject;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    //Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    //calculator.setStartPointCalculator(&startPointCalculator);
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());
    calculator.setNoise(noise);
    //startPointCalculator.setInputStartPoint(startPoint);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], testData.getResultsTwoParam()[0], 5);
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsTwoParam()[1], 2e-1);
}

TEST(OxCalculatorT2Truncation, blood_3samples) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();
    TYPE noise[] = {2.3, 1.2, 3.2, 1.1, 4.3, 2.3, 1.2, 3.2, 1.1, 4.3, 3.4};
    //TYPE startPoint[] = { 500, 80 };

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> functionsObject;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    //Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    //calculator.setStartPointCalculator(&startPointCalculator);
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());
    //startPointCalculator.setInputStartPoint(startPoint);
    calculator.setNoise(noise);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], testData.getResultsTwoParam()[0], 5);
    EXPECT_NEAR(calculator.getResults()["T2"], testData.getResultsTwoParam()[1], 1);
}

TEST(OxCalculatorT2Truncation, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T2_blood_3samples.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();
    TYPE noise[] = {2.3, 1.2, 3.2, 1.1, 4.3, 2.3, 1.2, 3.2, 1.1, 4.3, 3.4};

    // init the necessary objects
    Ox::ModelT2TwoParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    //Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitterAmoebaVnl);
    //calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(testData.getEchoTimesPtr());
    calculator.setSigMag(testData.getSignalMagPtr());
    calculator.setNoise(noise);

    calculator.setMeanCutOff(123);

    Ox::CalculatorT2Truncation<TYPE> calculatorCopy = calculator;

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
#endif // USE_PRIVATE_NR2