/*!
 * \file OxFitterAmoebaPrivateNr2_test.cpp
 * \author Konrad Werys
 * \date 2018/08/07
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFitterAmoebaPrivateNr2.h"
#include "OxFunctionsT1Basic.h"

TEST(OxFitterAmoebaPrivateNr2, performFitting) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());

    Ox::FitterAmoebaPrivateNr2<TYPE> fitterAmoebaNr2;
    fitterAmoebaNr2.setFunctionsT1(&functionsObject);
    fitterAmoebaNr2.setParameters(params);

    fitterAmoebaNr2.setVerbose(false);
    fitterAmoebaNr2.setTrace(false);

    fitterAmoebaNr2.performFitting();

    EXPECT_NEAR(params[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(params[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(params[2], testData.getResultsMolli()[2], 1e-2);
}

TEST(OxFitterAmoebaPrivateNr2, copyConstructor) {
    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());

    Ox::FitterAmoebaPrivateNr2<TYPE> fitter;
    fitter.setFunctionsT1(&functionsObject);
    fitter.setMaxFunctionEvals(123);
    fitter.setFTolerance(321);
    fitter.setParameters(params);

    // copy constructor
    Ox::FitterAmoebaPrivateNr2<TYPE> fitterCopy = fitter;

    // MaxFunctionEvals should be the same
    EXPECT_EQ( fitterCopy.getMaxFunctionEvals(), fitter.getMaxFunctionEvals());
    EXPECT_EQ( fitterCopy.getFTolerance(), fitter.getFTolerance());

    // FunctionsT1 was not declared for the copy, so it should be empty
    EXPECT_FALSE( fitterCopy.getFunctionsT1());
    EXPECT_NE( fitterCopy.getFunctionsT1(), fitter.getFunctionsT1());
}

#endif // USE_PRIVATE_NR2