/*!
 * \file OxFitterLevenbergMarquardtVnl_test.cpp
 * \author Konrad Werys
 * \date 2018/07/31
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFunctionsT1Basic.h"
#include "OxFitterLevenbergMarquardtVnl.h"

TEST(OxFitterLevenbergMarquardtVnl, performFitting) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());

    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterLevenbergMarquardtVnl;
    fitterLevenbergMarquardtVnl.setFunctionsT1(&functionsObject);
    functionsObject.setParameters(params);

    fitterLevenbergMarquardtVnl.setVerbose(true);
    fitterLevenbergMarquardtVnl.setTrace(true);

    fitterLevenbergMarquardtVnl.performFitting();

    EXPECT_NEAR(params[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(params[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(params[2], testData.getResultsMolli()[2], 1e-2);

}

TEST(OxFitterLevenbergMarquardtVnl, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());
    functionsObject.setParameters(params);

    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterAmoebaVnl;
    fitterAmoebaVnl.setFunctionsT1(&functionsObject);
    fitterAmoebaVnl.setMaxFunctionEvals(123);

    // copy constructor
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterAmoebaVnlCopy = fitterAmoebaVnl;

    // MaxFunctionEvals should be the same
    EXPECT_EQ( fitterAmoebaVnlCopy.getMaxFunctionEvals(), fitterAmoebaVnl.getMaxFunctionEvals());

    // FunctionsT1 was not declared for the copy, so it should be empty
    EXPECT_FALSE( fitterAmoebaVnlCopy.getFunctionsT1());
    EXPECT_NE( fitterAmoebaVnlCopy.getFunctionsT1(), fitterAmoebaVnl.getFunctionsT1());

    // new FunctionsAdaptedToVnl should NOT be copied in the constructor
    EXPECT_FALSE( fitterAmoebaVnlCopy.getFunctionsAdaptedToVnl());
}