/*!
 * \file OxStartPointCalculatorShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/22
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"
#include "OxTestData.h"

#ifdef USE_PRIVATE_NR2
#include "OxStartPointCalculatorShmolli.h"

TEST(OxStartPointCalculatorShmolli, calculateStartPoint) {

    typedef double TYPE;

    char filePath [] = "testData/blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE calculatedStartPoint[3] = {0,0,0};
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    startPointCalculator.setInvTimes(testData.getInvTimesPtr());
    startPointCalculator.setSigMag(testData.getSignalMagPtr());
    startPointCalculator.setSigns(testData.getSignsPtr());
    startPointCalculator.setNSamples(nSamples);
    startPointCalculator.setCalculatedStartPoint(calculatedStartPoint);

    startPointCalculator.calculateStartPoint();

    EXPECT_NEAR(calculatedStartPoint[0], 61.7    , 1e-2);
    EXPECT_NEAR(calculatedStartPoint[1], 123.4   , 1e-2);
    EXPECT_NEAR(calculatedStartPoint[2], 1549.04 , 1e-2);
}

#endif // USE_PRIVATE_NR2