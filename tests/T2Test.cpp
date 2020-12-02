//
// Created by Konrad Werys on 24.11.20.
//

#include "CmakeConfigForTomato.h"

#ifdef USE_PRIVATE_NR2
#ifdef USE_VNL

#include <gtest/gtest.h>
#include "OxCalculatorT2.h"
#include "OxCalculatorT2Linear.h"
#include "OxCalculatorT2Truncation.h"
#include "OxModelT2TwoParamScale.h"
#include "OxModelT2ThreeParam.h"
#include "OxFitterLevenbergMarquardtVnl.h"

TEST(tomato, greetingT2) {
    printf("\nThank you for using TOMATO v%d.%d.%d!\n", Tomato_VERSION_MAJOR, Tomato_VERSION_MINOR, Tomato_VERSION_PATCH);
}

/**
 * correct values taken from Matlab:
    sig = [679, 610, 536];
    t = [0, 25, 55];
    model = @(r,t)r(1) * exp(-t/r(2));
    start = [sig(end), 5];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctB = beta(1)
    correctT2 = beta(2)
    correctDeltaB = sqrt(CovB(1))
    correctDeltaT2 = sqrt(CovB(4))
 */
TEST(tomato, T2NonLinearThreeSamples) {

    typedef double TYPE;

    TYPE signal[] = {679, 610, 536 };
    TYPE time[] = { 0, 25, 55 };
    int nSamples = 3;

    double tolerance = 1e-4;

    double correctB = 679.06579;
    double correctT2 = 232.60309;
    double correctR2 = 0.99999;
    double correctDeltaB = 0.15118;
    double correctDeltaT2 = 0.38413;

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

    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
}

/**
 * correct values taken from Matlab:
    sig = [679, 610, 536];
    t = [0, 25, 55];
    mdl = fitlm(t, log(sig))
    a = mdl.Coefficients.Estimate(1)
    b = mdl.Coefficients.Estimate(2)
    correctB = exp(a)
    correctT2 = -1/b;
    correctDeltaB = exp(a) * mdl.Coefficients.SE(1)
    correctDeltaT2 = 1/b/b * mdl.Coefficients.SE(2)
 */
TEST(tomato, T2LinearThreeSamples) {

    typedef double TYPE;

    TYPE signal[] = { 679, 610, 536 };
    TYPE time[] = { 0, 25, 55 };
    int nSamples = 3;

    double tolerance = 1e-4;

    double correctB = 679.08173;
    double correctT2 = 232.55123;
    double correctR2 = 0.99999;
    double correctDeltaB = 0.1646;
    double correctDeltaT2 = 0.3758;

    Ox::CalculatorT2Linear<TYPE> calculator;

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setEchoTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T2"], correctT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2"], correctR2, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
}

/**
 * correct values taken from Matlab:
    sig = [110, 75, 49, 35, 25, 18, 11, 8];
    t = [3, 6, 9, 12, 15, 18, 21, 24];
    model = @(r,t)r(1) * exp(-t/r(2));
    start = [sig(1), 5];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctB = beta(1)
    correctT2 = beta(2)
    correctDeltaB = sqrt(CovB(1))
    correctDeltaT2 = sqrt(CovB(4))
 */
TEST(tomato, T2StarNoCorrectionEightSamples) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 25, 18, 11, 8};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctB = 159.9788;
    double correctT2 = 7.9055;
    double correctR2 = 0.9990;
    double correctDeltaB = 2.3782;
    double correctDeltaT2 = 0.1389;

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
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
}

/**
 * correct values taken from Matlab:
    sig = [110, 75, 49, 35, 25, 18, 11, 8];
    t = [3, 6, 9, 12, 15, 18, 21, 24];
    model = @(r,t)r(1) * exp(-t/r(2));
    start = [sig(1), 5];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctB = beta(1)
    correctT2 = beta(2)
    correctDeltaB = sqrt(CovB(1))
    correctDeltaT2 = sqrt(CovB(4))
 */
TEST(tomato, T2StarTruncation_highR2_lowT2) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 25, 18, 11, 8};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctB = 159.9788;
    double correctT2 = 7.9055;
    double correctR2 = 0.9990;
    double correctDeltaB = 2.3782;
    double correctDeltaT2 = 0.1389;
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
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
    EXPECT_NEAR(calculator.getResults()["nSamplesUsed"], correctNSamplesUsed, tolerance);
    EXPECT_NEAR(calculator.getResults()["exitCondition"], correctExitCondition, tolerance);
}

/**
 * correct values taken from Matlab:
    sig = [110, 75, 49, 35, 25, 18, 11, 8];
    t = [3, 6, 9, 12, 15, 18, 21, 24];
    model = @(r,t)r(1) + r(2) * exp(-t/r(3));
    start = [sig(end), sig(1), 7];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctOffset = beta(1)
    correctB = beta(2)
    correctT2 = beta(3)
    correctDeltaOffset = sqrt(CovB(1))
    correctDeltaB = sqrt(CovB(5))
    correctDeltaT2 = sqrt(CovB(9))
 */
TEST(tomato, T2StarOffsetEightSamples) {

    typedef double TYPE;

    TYPE signal[] = {110, 75, 49, 35, 25, 18, 11, 8};
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};
    int nSamples = 8;

    double tolerance = 1e-4;

    double correctOffset = 1.85051;
    double correctB = 160.6292;
    double correctT2 = 7.56479;
    double correctR2 = 0.9991;
    double correctDeltaOffset = 1.5100;
    double correctDeltaB = 2.4652;
    double correctDeltaT2 = 0.3037;

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
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaOffset, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT2"], correctDeltaT2, tolerance);
}

#endif // USE_VNL
#endif // USE_PRIVATE_NR2