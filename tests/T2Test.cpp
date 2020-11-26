//
// Created by Konrad Werys on 24.11.20.
//

#include <gtest/gtest.h>

#include "CmakeConfigForTomato.h"

#ifdef USE_PRIVATE_NR2

#include "OxCalculatorT2.h"
#include "OxCalculatorT2Linear.h"
#include "OxCalculatorT2Truncation.h"
#include "OxModelT2TwoParam.h"
#include "OxModelT2TwoParamScale.h"
#include "OxModelT2ThreeParam.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxStartPointCalculatorBasic.h"

TEST(tomatoGreeting, threeSamples) {
    printf("\nThank you for using TOMATO v%d.%d.%d!\n", Tomato_VERSION_MAJOR, Tomato_VERSION_MINOR, Tomato_VERSION_PATCH);
}

TEST(cviOxCalculatorT2NonLinear, threeSamples) {

    typedef double TYPE;

    TYPE signal[] = {679, 610, 536 };
    TYPE time[] = { 0, 25, 55 };
    int nSamples = 3;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], 679.08, 1e-1);
    EXPECT_NEAR(calculator.getResults()["T2"], 232.55, 1e-1);
}

TEST(cviOxCalculatorT2Linear, threeSamples) {

    typedef double TYPE;

    TYPE signal[] = {679, 610, 536 };
    TYPE time[] = { 0, 25, 55 };
    int nSamples = 3;

    Ox::CalculatorT2Linear<TYPE> calculator;

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], 679.08, 1e-1);
    EXPECT_NEAR(calculator.getResults()["T2"], 232.55, 1e-1);
}

TEST(cviOxCalculatorT2StarNoCorrection, eightSamples) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 25, 18, 11, 8};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctB = 159.9788;
    double correctT2 = 7.9055;
    double correctR2 = 0.9990;

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
}

TEST(cviOxCalculatorT2StarTruncation, highR2_lowT2) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 25, 18, 11, 8};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctB = 159.9788;
    double correctT2 = 7.9055;
    double correctR2 = 0.9990;
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

    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
    EXPECT_NEAR(calculator.getResults()["nSamplesUsed"], correctNSamplesUsed, tolerance);
    EXPECT_NEAR(calculator.getResults()["exitCondition"], correctExitCondition, tolerance);
}

TEST(cviOxCalculatorT2StarOffset, eightSamples) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 25, 18, 11, 8};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctOffset = 1.85051;
    double correctB = 108.0425;
    double correctT2 = 7.56479;
    double correctR2 = 0.9991;

    // init the necessary objects
    Ox::ModelT2ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(echoTimes);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctOffset, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
}

#endif // USE_PRIVATE_NR2