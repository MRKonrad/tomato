/*!
 * \file OxFitterAmoebaVnl_test.cpp
 * \author Konrad Werys
 * \date 2018/07/31
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxModelT1ThreeParam.h"
#include "OxFitterAmoebaVnl.h"

#include "CmakeConfigForTomato.h"

#ifdef USE_VNL
#ifdef USE_YAML
TEST(OxFitterAmoebaVnl, performFitting) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::ModelT1ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalPtr());

    Ox::FitterAmoebaVnl<TYPE> fitter;
    fitter.setModel(&model);
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

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::ModelT1ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalPtr());


    Ox::FitterAmoebaVnl<TYPE> fitter;
    fitter.setModel(&model);
    fitter.setParameters(params);
    fitter.setMaxFunctionEvals(123);
    fitter.setFTolerance(321);

    // copy constructor
    Ox::FitterAmoebaVnl<TYPE> fitterCopy = fitter;

    // MaxFunctionEvals should be the same
    EXPECT_EQ( fitterCopy.getMaxFunctionEvals(), fitter.getMaxFunctionEvals());
    EXPECT_EQ( fitterCopy.getFTolerance(), fitter.getFTolerance());

    // Model was not declared for the copy, so it should be empty
    EXPECT_FALSE( fitterCopy.getModel());
    EXPECT_NE( fitterCopy.getModel(), fitter.getModel());

    // new FunctionsAdaptedToVnl should NOT be copied in the constructor
    EXPECT_FALSE( fitterCopy.getModelAdaptedToVnl());

}

#ifndef _WIN32

TEST(OxFitterAmoebaVnl, disp) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::ModelT1ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalPtr());

    Ox::FitterAmoebaVnl<TYPE> fitter;
    fitter.setModel(&model);
    fitter.setParameters(params);

    testing::internal::CaptureStdout();
    fitter.disp();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.size(), 0);
}

#endif // _WIN32

#endif // USE_YAML
#endif // USE_VNL