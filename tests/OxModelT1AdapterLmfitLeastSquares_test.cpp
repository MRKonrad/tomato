/*!
 * \file OxModelT1AdapterLmfitLeastSquares_test.cpp
 * \author Konrad Werys
 * \date 2020/01/13
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_LMFIT
#ifdef USE_YAML

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxModelT1ThreeParam.h"
#include "OxModelT1AdapterLmfitLeastSquares.h"


TEST(OxModelT1AdapterLmfitLeastSquares, f) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);

    int nSamples = testData.getNSamples();

    TYPE params[3] = {0, 0, 0};
    TYPE *residualsLmfit = new TYPE[nSamples];

    Ox::ModelT1ThreeParam<TYPE> model;
    model.setNSamples(nSamples);
    model.setInvTimes(testData.getInvTimesPtr());
    model.setSignal(testData.getSignalMagPtr());

    Ox::ModelT1AdapterLmfitLeastSquares<TYPE>::calcLSResiduals(params, nSamples, &model, residualsLmfit, 0);

    for (int i = 0; i < nSamples; i++){
        EXPECT_DOUBLE_EQ(residualsLmfit[i], -testData.getSignalMag()[i]);
    }
    delete [] residualsLmfit;
}

#endif // USE_YAML
#endif // USE_Lmfit