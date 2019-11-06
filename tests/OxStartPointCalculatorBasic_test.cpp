/*!
 * \file OxStartPointCalculatorBasic_test.cpp
 * \author Konrad Werys
 * \date 2018/08/22
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxStartPointCalculatorBasic.h"

TEST(OxStartPointCalculatorBasic, calculateStartPoint_2d) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    int nDims = 2;
    TYPE const startPointGT[] = {3, 4};
    TYPE calculatedStartPoint[] = {0, 0};
    KWUtil::copyArrayToArray(nDims, calculatedStartPoint, startPointGT);

    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;

    startPointCalculator.setNDims(nDims);
    startPointCalculator.setInputStartPoint(startPointGT);

    startPointCalculator.setInvTimes(testData.getInvTimesPtr());
    startPointCalculator.setSigMag(testData.getSignalMagPtr());
    startPointCalculator.setSigns(testData.getSignsPtr());
    startPointCalculator.setNSamples(nSamples);
    startPointCalculator.setCalculatedStartPoint(calculatedStartPoint);

    startPointCalculator.calculateStartPoint();

    EXPECT_EQ(calculatedStartPoint[0], 3);
    EXPECT_EQ(calculatedStartPoint[1], 4);

}

TEST(OxStartPointCalculatorBasic, calculateStartPoint_3d) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    int nDims = 3;
    TYPE const startPointGT[] = {9, 4, 2};
    TYPE calculatedStartPoint[] = {0, 0, 0};
    KWUtil::copyArrayToArray(nDims, calculatedStartPoint, startPointGT);

    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;

    startPointCalculator.setNDims(nDims);
    startPointCalculator.setInputStartPoint(startPointGT);

    startPointCalculator.setInvTimes(testData.getInvTimesPtr());
    startPointCalculator.setSigMag(testData.getSignalMagPtr());
    startPointCalculator.setSigns(testData.getSignsPtr());
    startPointCalculator.setNSamples(nSamples);
    startPointCalculator.setCalculatedStartPoint(calculatedStartPoint);

    startPointCalculator.calculateStartPoint();

    EXPECT_EQ(calculatedStartPoint[0], 9);
    EXPECT_EQ(calculatedStartPoint[1], 4);
    EXPECT_EQ(calculatedStartPoint[2], 2);

}