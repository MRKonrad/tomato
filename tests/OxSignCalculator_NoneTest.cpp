/*!
 * \file OxSignCalculatorNone_test.cpp
 * \author Konrad Werys
 * \date 2019/11/13
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "OxModelT1ThreeParam.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT1WithSignCheck.h"

#ifdef USE_VNL
TEST(OxSignCalculator, None) {

        typedef double TYPE;

        char filePath [] = "testData/T1_blood.yaml";
        Ox::TestData<TYPE> testData(filePath);
        int nSamples = testData.getNSamples();

        // init the necessary objects
        Ox::ModelT1ThreeParam<TYPE> model;
        Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
        Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
        Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1Molli;

        // configure
        calculatorT1Molli.setModel(&model);
        calculatorT1Molli.setFitter(&fitter);
        calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

        // set the data
        calculatorT1Molli.setNSamples(nSamples);
        calculatorT1Molli.setInvTimes(testData.getInvTimesPtr());
        calculatorT1Molli.setSigMag(testData.getSignalPtr());

        calculatorT1Molli.calculate();

        EXPECT_NEAR(calculatorT1Molli.getResults()["A"], testData.getResultsMolli()[0], 1e-2);
        EXPECT_NEAR(calculatorT1Molli.getResults()["B"], testData.getResultsMolli()[1], 1e-2);
        EXPECT_NEAR(calculatorT1Molli.getResults()["T1star"], testData.getResultsMolli()[2], 1e-2);
    }
#endif



