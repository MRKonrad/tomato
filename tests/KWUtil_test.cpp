/*!
 * \file KWUtil_test.cpp
 * \author Konrad Werys
 * \date 2018/10/04
 */

#include "KWUtil.h"

#include "gtest/gtest.h"

TEST(KWUtil, dicomTime2Seconds) {

    std::string dicomTimeString = "141650.92";
    double dicomTimeSeconds = 14*3600 + 16*60 + 50.92;

    double result = KWUtil::dicomTime2Seconds<double>(dicomTimeString);
    EXPECT_DOUBLE_EQ(result, dicomTimeSeconds);
}

TEST(KWUtil, calcMatrixInverse3x3) {

    double matrix[9] = {2, 2, 2, 4, 5, 4, 7, 8, 2};
    double invMatrixCorrect[9] = { 2.2, -1.2, 0.2, -2, 1, 0, 0.3, 0.2, -0.2};
    double invMatrixCalculated[9];

    EXPECT_EQ(KWUtil::calcMatrixInverse3x3(matrix, invMatrixCalculated), 0); // EXIT_SUCCESS

    EXPECT_DOUBLE_EQ(invMatrixCalculated[0], invMatrixCorrect[0]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[1], invMatrixCorrect[1]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[2], invMatrixCorrect[2]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[3], invMatrixCorrect[3]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[4], invMatrixCorrect[4]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[5], invMatrixCorrect[5]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[6], invMatrixCorrect[6]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[7], invMatrixCorrect[7]);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[8], invMatrixCorrect[8]);
}

TEST(KWUtil, calcMatrixInverse3x3_detZero) {

    double matrix[9] = {2, 2, 2, 3, 3, 3, 1, 1, 1};
    double invMatrixCalculated[9];

    EXPECT_EQ(KWUtil::calcMatrixInverse3x3(matrix, invMatrixCalculated), 1); // EXIT_FAILURE

    EXPECT_DOUBLE_EQ(invMatrixCalculated[0], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[1], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[2], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[3], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[4], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[5], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[6], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[7], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[8], 0);
}


