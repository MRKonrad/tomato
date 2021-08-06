/*!
 * \file OxCalculatorT2Truncation_test.cpp
 * \author Konrad Werys
 * \date 2019/11/01
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2

#include "OxModelT2TwoParamScale.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT2Truncation.h"

#ifdef USE_VNL
TEST(OxCalculatorT2Truncation, highR2_lowT2) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 25, 18, 11, 8};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctT2 = 7.9055;
    double correctR2 = 0.9990;
    double correctFe = 3.6119;
    double correctNSamplesUsed = 8;
    double correctExitCondition = 1;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
    EXPECT_NEAR(calculator.getResults()["Fe"], correctFe, tolerance);
    EXPECT_NEAR(calculator.getResults()["nSamplesUsed"], correctNSamplesUsed, tolerance);
    EXPECT_NEAR(calculator.getResults()["exitCondition"], correctExitCondition, tolerance);
}

TEST(OxCalculatorT2Truncation, highR2_highT2) {

    typedef double TYPE;

    TYPE signal[] = {106, 94, 84, 74, 66, 58, 52, 46};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctT2 = 25.1418;
    double correctR2 = 0.9998;
    double correctFe = 0.8805;
    double correctNSamplesUsed = 8;
    double correctExitCondition = 1;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
    EXPECT_NEAR(calculator.getResults()["Fe"], correctFe, tolerance);
    EXPECT_NEAR(calculator.getResults()["nSamplesUsed"], correctNSamplesUsed, tolerance);
    EXPECT_NEAR(calculator.getResults()["exitCondition"], correctExitCondition, tolerance);
}

TEST(OxCalculatorT2Truncation, truncationInWork) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 24, 25, 26, 25};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctB = 162.6716;
    double correctDeltaB = 2.9064;
    double correctT2 = 7.6755;
    double correctDeltaT2 = 0.1864;
    double correctR2 = 0.9991;
    double correctFe = 3.7444;
    double correctDeltaFe = 0.1109;
    double correctNSamplesUsed = 4;
    double correctExitCondition = 3;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
    EXPECT_NEAR(calculator.getResults()["Fe"], correctFe, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaFe"], correctDeltaFe, tolerance);
    EXPECT_NEAR(calculator.getResults()["nSamplesUsed"], correctNSamplesUsed, tolerance);
    EXPECT_NEAR(calculator.getResults()["exitCondition"], correctExitCondition, tolerance);
}

TEST(OxCalculatorT2Truncation, lowR2_highT2) {

    typedef double TYPE;

    TYPE signal[] = {109, 93, 86, 72, 64, 60, 52, 49};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctT2 = 25.1101;
    double correctR2 = 0.9897;
    double correctFe = 0.8818;
    double correctNSamplesUsed = 8;
    double correctExitCondition = 4;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
    EXPECT_NEAR(calculator.getResults()["Fe"], correctFe, tolerance);
    EXPECT_NEAR(calculator.getResults()["nSamplesUsed"], correctNSamplesUsed, tolerance);
    EXPECT_NEAR(calculator.getResults()["exitCondition"], correctExitCondition, tolerance);
}

TEST(OxCalculatorT2Truncation, calculateFitError) {

    typedef double TYPE;

    TYPE signal[] = {109, 93, 86, 72, 64, 60, 52, 49};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctDeltaB = 2.4428;
    double correctDeltaT2 = 1.0852;
    double correctDeltaFe = 0.0465;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaFe"], correctDeltaFe, tolerance);
}

TEST(OxCalculatorT2Truncation, startPointCalculator) {

    typedef double TYPE;

    TYPE signal[] = {109, 93, 86, 72, 64, 60, 52, 49};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctDeltaB = 2.4428;
    double correctDeltaT2 = 1.0852;
    double correctDeltaFe = 0.0465;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitter;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaFe"], correctDeltaFe, tolerance);
}

TEST(OxCalculatorT2Truncation, copyConstructor) {

    typedef double TYPE;

    TYPE signal[] = {109, 93, 86, 72, 64, 60, 52, 49};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::CalculatorT2Truncation<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitterAmoebaVnl);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

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