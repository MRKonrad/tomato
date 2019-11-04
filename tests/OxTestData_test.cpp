/*!
 * \file OxTestData_test.cpp
 * \author Konrad Werys
 * \date 2018/08/19
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "TomatoParser.h"

TEST(OxTestData, read_file) {

    typedef double TYPE;
    char filePath [] = "testData/T1_blood.yaml";

    Ox::TestData<TYPE> testData(filePath);

    TYPE signalMag[7] =      {   55,   49,   29,   22,   51,   61,   64};
    TYPE signalPha[7] =      { 3348, 3341, 3310, -843, -767, -777, -768};
    TYPE signs[7] =          {   -1,   -1,   -1,    1,    1,    1,    1};
    TYPE invTimes[7] =       {  100,  180,  260, 1715, 3337, 4907, 6455};
    TYPE resultsMolli[3] =   { 65.8968, 124.079, 1571.81};
    TYPE resultsShmolli[3] = { 66.73, 129.59, 1601.5 };

    std::vector<TYPE> signalMagVec(signalMag, signalMag+7);
    std::vector<TYPE> signalPhaVec(signalPha, signalPha+7);
    std::vector<TYPE> signsVec(signs, signs+7);
    std::vector<TYPE> invTimesVec(invTimes, invTimes+7);
    std::vector<TYPE> resultsMolliVec(resultsMolli, resultsMolli+3);
    std::vector<TYPE> resultsShmolliVec(resultsShmolli, resultsShmolli+3);

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(testData.getSignalMag()[i], signalMagVec[i]);
        EXPECT_EQ(testData.getSignalPha()[i], signalPhaVec[i]);
        EXPECT_EQ(testData.getSigns()[i], signsVec[i]);
        EXPECT_EQ(testData.getInvTimes()[i], invTimesVec[i]);
    }
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(testData.getResultsMolli()[i], resultsMolliVec[i]);
        EXPECT_EQ(testData.getResultsShmolli()[i], resultsShmolliVec[i]);
    }
}
