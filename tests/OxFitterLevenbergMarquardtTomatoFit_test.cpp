/*!
 * \file OxFitterLevenbergMarquardtTomatoFit_test.cpp
 * \author Konrad Werys
 * \date 2020/01/13
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_TOMATOFIT

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxModelT1ThreeParam.h"
#include "OxFitterLevenbergMarquardtTomatoFit.h"

#ifdef USE_PRIVATE_NR2
#include "OxModelT1Shmolli.h"
#endif // USE_PRIVATE_NR2

TEST(OxFitterLevenbergMarquardtTomatoFit, performFitting) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE params[3] = {100, 200, 1200};

    Ox::ModelT1ThreeParam<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());

    Ox::FitterLevenbergMarquardtTomatoFit<TYPE> fitter;
    fitter.setModel(&functionsObject);
    fitter.setParameters(params);

    fitter.setVerbose(false);
    fitter.setTrace(false);

    fitter.performFitting();

    EXPECT_NEAR(params[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(params[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(params[2], testData.getResultsMolli()[2], 1e-2);
}

//#ifdef USE_PRIVATE_NR2
//
//TEST(OxFitterLevenbergMarquardtTomatoFit, performFitting_shmolli) {
//
//    typedef double TYPE;
//
//    char filePath [] = "testData/T1_blood.yaml";
//    Ox::TestData<TYPE> testData(filePath);
//    int nSamples = testData.getNSamples();
//
//    TYPE params[3] = {100, 200, 1200};
//
//    Ox::ModelT1Shmolli<TYPE> functionsObject;
//    functionsObject.setNSamples(nSamples);
//    functionsObject.setInvTimes(testData.getInvTimesPtr());
//    functionsObject.setSignal(testData.getSignalPtr());
//
//    Ox::FitterLevenbergMarquardtTomatoFit<TYPE> fitter;
//    fitter.setModel(&functionsObject);
//    fitter.setParameters(params);
//
//    fitter.setVerbose(false);
//    fitter.setTrace(false);
//
//    fitter.performFitting();
//
//    EXPECT_NEAR(params[0], testData.getResultsMolli()[0], 1e-2);
//    EXPECT_NEAR(params[1], testData.getResultsMolli()[1], 1e-2);
//    EXPECT_NEAR(params[2], testData.getResultsMolli()[2], 1e-2);
//}
//
//#endif // USE_PRIVATE_NR2
//
//TEST(OxFitterLevenbergMarquardtTomatoFit, copyConstructor) {
//
//    typedef double TYPE;
//
//    char filePath [] = "testData/T1_blood.yaml";
//    Ox::TestData<TYPE> testData(filePath);
//    int nSamples = testData.getNSamples();
//
//    TYPE params[3] = {100, 200, 1200};
//
//    Ox::ModelT1ThreeParam<TYPE> functionsObject;
//    functionsObject.setNSamples(nSamples);
//    functionsObject.setInvTimes(testData.getInvTimesPtr());
//    functionsObject.setSignal(testData.getSignalPtr());
//
//
//    Ox::FitterLevenbergMarquardtTomatoFit<TYPE> fitter;
//    fitter.setModel(&functionsObject);
//    fitter.setParameters(params);
//    fitter.setMaxFunctionEvals(123);
//    fitter.setFTolerance(321);
//
//    // copy constructor
//    Ox::FitterLevenbergMarquardtTomatoFit<TYPE> fitterCopy = fitter;
//
//    // MaxFunctionEvals should be the same
//    EXPECT_EQ( fitterCopy.getMaxFunctionEvals(), fitter.getMaxFunctionEvals());
//    EXPECT_EQ( fitterCopy.getFTolerance(), fitter.getFTolerance());
//
//    // Model was not declared for the copy, so it should be empty
//    EXPECT_FALSE( fitterCopy.getModel());
//    EXPECT_NE( fitterCopy.getModel(), fitter.getModel());
//
//}
//
//#ifndef _WIN32
//
//TEST(OxFitterLevenbergMarquardtTomatoFit, disp) {
//
//    typedef double TYPE;
//
//    char filePath [] = "testData/T1_blood.yaml";
//    Ox::TestData<TYPE> testData(filePath);
//    int nSamples = testData.getNSamples();
//
//    TYPE params[3] = {100, 200, 1200};
//
//    Ox::ModelT1ThreeParam<TYPE> functionsObject;
//    functionsObject.setNSamples(nSamples);
//    functionsObject.setInvTimes(testData.getInvTimesPtr());
//    functionsObject.setSignal(testData.getSignalPtr());
//
//    Ox::FitterLevenbergMarquardtTomatoFit<TYPE> fitter;
//    fitter.setModel(&functionsObject);
//    fitter.setParameters(params);
//
//    testing::internal::CaptureStdout();
//    fitter.disp();
//    std::string output = testing::internal::GetCapturedStdout();
//
//    EXPECT_NE(output.size(), 0);
//}
//
//#endif // _WIN32

#endif // USE_TOMATOFIT