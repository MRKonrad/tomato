/*!
 * \file OxFitterAmoebaVnl_test.cpp
 * \author Konrad Werys
 * \date 2018/07/31
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"

TEST(OxFitterAmoebaVnl, calcModelValueTest) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());

    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    fitterAmoebaVnl.setFunctionsT1(&functionsObject);
    functionsObject.setParameters(params);

    fitterAmoebaVnl.setVerbose(true);
    fitterAmoebaVnl.setTrace(true);

    fitterAmoebaVnl.performFitting();

    EXPECT_NEAR(functionsObject.getParameters()[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(functionsObject.getParameters()[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(functionsObject.getParameters()[2], testData.getResultsMolli()[2], 1e-2);

}