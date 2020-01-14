/*!
 * \file OxModelT1AdapterTomatoFitLeastSquares_test.cpp
 * \author Konrad Werys
 * \date 2020/01/13
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_TOMATOFIT

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxModelT1ThreeParam.h"
#include "OxModelT1AdapterTomatoFitLeastSquares.h"


TEST(OxModelT1AdapterTomatoFitLeastSquares, f) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);

    int nSamples = testData.getNSamples();

    TYPE params[3] = {10, 20, 1000};

    Ox::ModelT1ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    Ox::ModelT1AdapterTomatoFitLeastSquares<TYPE> modelTF;
    modelTF.setModel(&model);

    EXPECT_DOUBLE_EQ(modelTF.getValue(params, 100), -8.0967483607191895);

}

#endif //USE_TOMATOFIT