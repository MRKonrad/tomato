/*!
 * \file OxStartPointCalculatorDefault3Dims_test.cpp
 * \author Konrad Werys
 * \date 2018/08/22
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxStartPointCalculatorDefault3Dims.h"

TEST(OxStartPointCalculatorDefault3Dims, calculateStartPoint) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE calculatedStartPoint[3] = {0,0,0};
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    startPointCalculator.setInvTimes(testData.getInvTimesPtr());
    startPointCalculator.setSigMag(testData.getSignalMagPtr());
    startPointCalculator.setSigns(testData.getSignsPtr());
    startPointCalculator.setNSamples(nSamples);
    startPointCalculator.setCalculatedStartPoint(calculatedStartPoint);

    startPointCalculator.calculateStartPoint();

    EXPECT_EQ(calculatedStartPoint[0], 100);
    EXPECT_EQ(calculatedStartPoint[1], 200);
    EXPECT_EQ(calculatedStartPoint[2], 1000);

}