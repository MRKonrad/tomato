/*!
 * \file OxCalculatorT1Shmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/02
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"

#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1Shmolli.h"
#include "OxFunctionsT1Shmolli.h"
#include "OxFitterAmoebaPrivateNr2.h"
#include "OxSignCalculatorShmolli.h"
#include "OxStartPointCalculatorShmolli.h"

//#ifdef USE_VNL
//#include "OxSignCalculatorNoSign.h"
//#include "OxFunctionsT1ThreeParam.h"
//#include "OxFitterAmoebaVnl.h"
//#include "OxFitterLevenbergMarquardtVnl.h"
//#include "OxSignCalculatorRealImag.h"
//#include "OxStartPointCalculatorShmolli.h"

Ox::CalculatorT1Results<double> CalculateShmolli(
        int nSamples, double* sigMag, double* sigPha, double* invTimes) {

    // initialise the necessary objects
    Ox::CalculatorT1Shmolli <double> calculatorT1Shmolli;
    Ox::FunctionsT1Shmolli <double> functionsObject;
    Ox::FitterAmoebaPrivateNr2 <double> fitter;
    Ox::SignCalculatorShmolli <double> signCalculator;
    Ox::StartPointCalculatorShmolli <double> startPointCalculator;

    // configure
    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
    calculatorT1Shmolli.setFitter(&fitter);
    calculatorT1Shmolli.setSignCalculator(&signCalculator);
    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1Shmolli.setNSamples(nSamples);
    calculatorT1Shmolli.setInvTimes(invTimes);
    calculatorT1Shmolli.setSigMag(sigMag);
    calculatorT1Shmolli.setSigPha(sigPha);

    calculatorT1Shmolli.calculate();

    return calculatorT1Shmolli.getResults();
}

TEST(OxCalculatorT1Shmolli, calculate_anything) {
    int nSamples = 7;
    double sigMag[7] = {49,   40,   26,   42,   59,   62,   63};
    double sigPha[7] = {3918, 3560, 3587, -809, -729, -784, -795};
    double invTimes[7] = {100,  180,  260, 1715, 3337, 4907, 6455};

    Ox::CalculatorT1Results<double> results = CalculateShmolli(nSamples, sigMag, sigPha, invTimes);

    //results.disp();

    EXPECT_NE(results.T1, 0);
}

//#endif // USE_VNL
#endif // USE_PRIVATE_NR2

