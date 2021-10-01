//
// Created by Konrad Werys on 27.11.20.
//

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2
#ifdef USE_VNL

#include <gtest/gtest.h>
#include "OxCalculatorT1WithSignCheck.h"
#include "OxCalculatorT1Shmolli.h"
#include "OxModelT1TwoParam.h"
#include "OxModelT1ThreeParam.h"
#include "OxModelT1Shmolli.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorShmolli.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxStartPointCalculatorShmolli.h"

TEST(tomato, greetingT1) {
    printf("\nThank you for using TOMATO v%d.%d.%d!\n", Tomato_VERSION_MAJOR, Tomato_VERSION_MINOR, Tomato_VERSION_PATCH);
}

/**
 * correct values taken from Matlab:
    sig = [-49,   -40,   -26,   42,   59,   62,   63];
    t = [100,  180,  260, 1715, 3337, 4907, 6455];
    model = @(r,t)r(1) - r(2) * exp(-t/r(3));
    start = [sig(end), sig(end)*2, 1000];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctA = beta(1)
    correctB = beta(2)
    correctT1star = beta(3)
    correctT1 = beta(3)*(beta(2)/beta(1)-1)
    correctDeltaA = sqrt(CovB(1))
    correctDeltaB = sqrt(CovB(5))
    correctDeltaT1star = sqrt(CovB(9))
 */
TEST(tomato, T1Molli) {

    typedef double TYPE;

    TYPE signal[] = { -49,   -40,   -26,   42,   59,   62,   63};
    TYPE time[] = { 100,  180,  260, 1715, 3337, 4907, 6455 };
    int nSamples = 7;

    double tolerance = 1e-2;

    double correctA = 62.4225;
    double correctB = 122.5215;
    double correctT1star = 919.9441;
    double correctT1 = 885.7032;
    double correctR2Abs = 0.9845;
    double correctDeltaA = 1.6070;
    double correctDeltaB = 2.4948;
    double correctDeltaT1star = 77.9145;
    double correctDeltaT1 = 158.2645;
    double correctTimeFlip = 0;

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1"], correctT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1"], correctDeltaT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["timeFlip"], correctTimeFlip, tolerance);
}

/**
 * correct values taken from Matlab:
    sig = [-49,   -40,   -26,   42,   59,   62,   63];
    t = [100,  180,  260, 1715, 3337, 4907, 6455];
    model = @(r,t)r(1) - r(2) * exp(-t/r(3));
    start = [sig(end), sig(end)*2, 1000];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctA = beta(1)
    correctB = beta(2)
    correctT1star = beta(3)
    correctT1 = beta(3)*(beta(2)/beta(1)-1)
    correctDeltaA = sqrt(CovB(1))
    correctDeltaB = sqrt(CovB(5))
    correctDeltaT1star = sqrt(CovB(9))
 */
TEST(tomato, T1Molli_calculateSign) {

    typedef double TYPE;

    TYPE signal[] = { 49,   40,   26,   42,   59,   62,   63};
    TYPE time[] = { 100,  180,  260, 1715, 3337, 4907, 6455 };
    int nSamples = 7;

    double tolerance = 1e-2;

    double correctA = 62.4225;
    double correctB = 122.5215;
    double correctT1star = 919.9441;
    double correctT1 = 885.7032;
    double correctR2Abs = 0.9845;
    double correctDeltaA = 1.6070;
    double correctDeltaB = 2.4948;
    double correctDeltaT1star = 77.9145;
    double correctDeltaT1 = 158.2645;
    double correctTimeFlip = 3;

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1"], correctT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1"], correctDeltaT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["timeFlip"], correctTimeFlip, tolerance);
}

TEST(tomato, T1Shmolli_calculateSignWithoutPhase) {

    typedef double TYPE;

    TYPE signal[] = { 49,   40,   26,   42,   59,   62,   63};
    TYPE time[] = { 100,  180,  260, 1715, 3337, 4907, 6455 };
    int nSamples = 7;

    double tolerance = 1e-2;

    double correctA = 62.9474;
    double correctB = 124.1296;
    double correctT1star = 964.3718;
    double correctT1 = 937.3275;
    double correctR2Abs = 0.9998;
    double correctDeltaA = 0.1144;
    double correctDeltaB = 0.1775;
    double correctDeltaT1star = 5.5996;
    double correctDeltaT1 = 11.6167;
    double correctTimeFlip = 3;

    // init the necessary objects
    Ox::ModelT1Shmolli<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1"], correctT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1"], correctDeltaT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["timeFlip"], correctTimeFlip, tolerance);
}

// results from tomato
TEST(tomato, T1Shmolli_calculateSignWithoutPhase_5samples) {

    typedef double TYPE;

    TYPE signal[] = {
        95.665639445300456,
        53.529532614278374,
        13.85130970724191,
        128.0945043656908,
        156.30662557781201,
        159.62223934257833,
        160.15511042629686 };
    TYPE time[] = { 100, 180, 260, 1000, 1900, 2800, 3700 };
    int nSamples = 7;

    double tolerance = 1e-2;

    double correctA = 160.2188;
    double correctB = 322.2685;
    double correctT1star = 433.5508;
    double correctT1 = 438.5053;
    double correctR2Abs = 0.9998;
    double correctDeltaA = 0.0674;
    double correctDeltaB = 0.1775;
    double correctDeltaT1star = 0.8256;
    double correctDeltaT1 = 1.7039;
    double correctTimeFlip = 1;

    // init the necessary objects
    Ox::ModelT1Shmolli<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1"], correctT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1"], correctDeltaT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["timeFlip"], correctTimeFlip, tolerance);
}

TEST(tomato, T1Shmolli_calculateSignWithoutPhase_6samples) {

    typedef double TYPE;

    TYPE signal[] = {
        197.59609868043603,
        135.06540447504304,
        23.955249569707401,
        24.962134251290877,
        62.294320137693632,
        111.68445209409064,
        140.4084911072863};
    TYPE time[] = { 100, 180, 260, 1000, 1900, 2800, 3700 };
    int nSamples = 7;

    double tolerance = 1e-2;

    double correctA = 169.0853;
    double correctB = 392.4758;
    double correctT1star = 1441.06949;
    double correctT1 = 1903.8977;
    double correctR2Abs = 0.9998;
    double correctDeltaA = 4.1456;
    double correctDeltaB = 4.083;
    double correctDeltaT1star = 43.2234;
    double correctDeltaT1 = 173.9150;
    double correctTimeFlip = 4;

    // init the necessary objects
    Ox::ModelT1Shmolli<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1"], correctT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1"], correctDeltaT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["timeFlip"], correctTimeFlip, tolerance);
}

TEST(tomato, T1Shmolli_calculateSignWithoutPhase_7samples) {

    typedef double TYPE;

    TYPE signal[] = {
        89.360373295046656,
        8.201722900215362,
        81.208183776022977,
        261.15290739411341,
        270.49353912419241,
        269.98456568557071,
        270.83237616654702,
    };
    TYPE time[] = { 100, 180, 260, 1000, 1900, 2800, 3700  };
    int nSamples = 7;

    double tolerance = 1e-2;

    double correctA = 270.6357;
    double correctB = 537.7456;
    double correctT1star = 249.7954;
    double correctT1 = 246.5412;
    double correctR2Abs = 0.9998;
    double correctDeltaA = 0.3358;
    double correctDeltaB = 1.8499;
    double correctDeltaT1star = 1.4237;
    double correctDeltaT1 = 3.7284;
    double correctTimeFlip = 1;

    // init the necessary objects
    Ox::ModelT1Shmolli<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1"], correctT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1"], correctDeltaT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["timeFlip"], correctTimeFlip, tolerance);
}

TEST(tomato, T1Shmolli_calculateSignWithPhase) {

    typedef double TYPE;

    TYPE signal[] = { 49,   40,   26,   42,   59,   62,   63};
    TYPE signalPhase[] = {3918, 3560, 3587, -809, -729, -784, -795};
    TYPE time[] = { 100,  180,  260, 1715, 3337, 4907, 6455 };
    int nSamples = 7;

    double tolerance = 1e-2;

    double correctA = 62.9474;
    double correctB = 124.1296;
    double correctT1star = 964.3718;
    double correctT1 = 937.3275;
    double correctR2Abs = 0.9998;
    double correctDeltaA = 0.1144;
    double correctDeltaB = 0.1775;
    double correctDeltaT1star = 5.5996;
    double correctDeltaT1 = 11.6167;
    double correctTimeFlip = 0;

    // init the necessary objects
    Ox::ModelT1Shmolli<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);
    calculator.setSigPha(signalPhase);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1"], correctT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1"], correctDeltaT1, tolerance);
    EXPECT_NEAR(calculator.getResults()["timeFlip"], correctTimeFlip, tolerance);
}

/**
 * correct values taken from Matlab:
    sig = [25,    48,    66,    87,   105,   123,   139,   157,   175,   190,   599];
    t = [83,   153,   223,   293,   363,   433,   503,   573,   643,   713, 10000];
    model = @(r,t)r(1) - r(2) * exp(-t/r(3));
    start = [sig(end), sig(end)*2, 1000];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctA = beta(1)
    correctB = beta(2)
    correctT1star = beta(3)
    correctT1 = beta(3)*(beta(2)/beta(1)-1)
    correctDeltaA = sqrt(CovB(1))
    correctDeltaB = sqrt(CovB(5))
    correctDeltaT1star = sqrt(CovB(9))
 */
TEST(tomato, T1Sasha_2param) {

    typedef double TYPE;

    TYPE signal[] = { 25,    48,    66,    87,   105,   123,   139,   157,   175,   190,   599};
    TYPE time[] = { 83,   153,   223,   293,   363,   433,   503,   573,   643,   713, 10000 };
    int nSamples = 11;

    double tolerance = 1e-2;

    double correctA = 602.0598;
    double correctT1star = 1889.6967;
    double correctR2Abs = 0.9998;
    double correctDeltaA = 1.0461;
    double correctDeltaT1star = 6.9727;

    // init the necessary objects
    Ox::ModelT1TwoParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
}

/**
 * correct values taken from Matlab:
    sig = [25,    48,    66,    87,   105,   123,   139,   157,   175,   190,   599];
    t = [83,   153,   223,   293,   363,   433,   503,   573,   643,   713, 10000];
    model = @(r,t)r(1) - r(2) * exp(-t/r(3));
    start = [sig(end), sig(end)*2, 1000];
    [beta,R,J,CovB,MSE,ErrorModelInfo] = nlinfit(t, sig, model, start);
    correctA = beta(1)
    correctB = beta(2)
    correctT1star = beta(3)
    correctT1 = beta(3)*(beta(2)/beta(1)-1)
    correctDeltaA = sqrt(CovB(1))
    correctDeltaB = sqrt(CovB(5))
    correctDeltaT1star = sqrt(CovB(9))
 */
TEST(tomato, T1Sasha_3param) {

    typedef double TYPE;

    TYPE signal[] = { 25,    48,    66,    87,   105,   123,   139,   157,   175,   190,   599};
    TYPE time[] = { 83,   153,   223,   293,   363,   433,   503,   573,   643,   713, 10000 };
    int nSamples = 11;

    double tolerance = 1e-2;

    double correctA = 601.9936;
    double correctB = 602.4841;
    double correctT1star = 1883.2353;
    double correctR2Abs = 0.9998;
    double correctDeltaA = 1.0913;
    double correctDeltaB = 1.3024;
    double correctDeltaT1star = 13.0698;

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(time);
    calculator.setSigMag(signal);

    calculator.calculate();

    EXPECT_NEAR(calculator.getResults()["A"], correctA, tolerance);
    EXPECT_NEAR(calculator.getResults()["B"], correctB, tolerance);
    EXPECT_NEAR(calculator.getResults()["T1star"], correctT1star, tolerance);
    EXPECT_NEAR(calculator.getResults()["R2Abs"], correctR2Abs, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaA"], correctDeltaA, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaB"], correctDeltaB, tolerance);
    EXPECT_NEAR(calculator.getResults()["deltaT1star"], correctDeltaT1star, tolerance);
}

#endif // USE_VNL
#endif // USE_PRIVATE_NR2