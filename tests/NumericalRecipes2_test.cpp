/*!
 * \file NumericalRecipes_test.cpp
 * \author Konrad Werys
 * \date 2018/08/03
 */

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_NR2

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxFitterAmoebaNr2.h"
#include "OxFunctionsT1Basic.h"

TEST(NumericalRecipes, doAnything) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    double params[3] = {100, 200, 1200};

    Ox::FunctionsT1Basic<TYPE> functionsObject;
    functionsObject.setNSamples(nSamples);
    functionsObject.setInvTimes(testData.getInvTimesPtr());
    functionsObject.setSignal(testData.getSignalPtr());

    Ox::FitterAmoebaNr2<TYPE> fitterAmoebaVnl;
    fitterAmoebaVnl.setFunctionsT1(&functionsObject);
    functionsObject.setParameters(params);

    fitterAmoebaVnl.setVerbose(true);
    fitterAmoebaVnl.setTrace(true);

    fitterAmoebaVnl.performFitting();

//    EXPECT_NEAR(params[0], testData.getResultsMolli()[0], 1e-2);
//    EXPECT_NEAR(params[1], testData.getResultsMolli()[1], 1e-2);
//    EXPECT_NEAR(params[2], testData.getResultsMolli()[2], 1e-2);
}

#endif // USE_NR2

