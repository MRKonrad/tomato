/*!
 * \file OxFitterAmoebaVnl_test.cpp
 * \author Konrad Werys
 * \date 2018/07/31
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"

#include "CmakeConfigForTomato.h"

#ifdef USE_VNL
TEST(OxFitterAmoebaVnl, performFitting) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());

    Ox::FitterAmoebaVnl<TYPE> fitter;
    fitter.setFunctionsT1(&functionsObject);
    fitter.setParameters(params);

    fitter.setVerbose(false);
    fitter.setTrace(false);

    fitter.performFitting();

    EXPECT_NEAR(params[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(params[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(params[2], testData.getResultsMolli()[2], 1e-2);

}

TEST(OxFitterAmoebaVnl, copyConstructor) {
    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());


    Ox::FitterAmoebaVnl<TYPE> fitter;
    fitter.setFunctionsT1(&functionsObject);
    fitter.setParameters(params);
    fitter.setMaxFunctionEvals(123);
    fitter.setFTolerance(321);

    // copy constructor
    Ox::FitterAmoebaVnl<TYPE> fitterCopy = fitter;

    // MaxFunctionEvals should be the same
    EXPECT_EQ( fitterCopy.getMaxFunctionEvals(), fitter.getMaxFunctionEvals());
    EXPECT_EQ( fitterCopy.getFTolerance(), fitter.getFTolerance());

    // FunctionsT1 was not declared for the copy, so it should be empty
    EXPECT_FALSE( fitterCopy.getFunctionsT1());
    EXPECT_NE( fitterCopy.getFunctionsT1(), fitter.getFunctionsT1());

    // new FunctionsAdaptedToVnl should NOT be copied in the constructor
    EXPECT_FALSE( fitterCopy.getFunctionsAdaptedToVnl());

}
#endif