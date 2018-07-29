/*!
 * \file OxFunctionsT1Basic_test.cpp
 * \author Konrad Werys
 * \date 2018/08/29
 */

#include "gtest/gtest.h"
#include "OxFunctionsT1Basic.h"

TEST(OxShmolli2, calcModelValueTest) {

    double params[3] = {100, 200, 1000};

    Ox::FunctionsT1Basic<double> FunctionsObject;
    FunctionsObject.setParameters(params);

    EXPECT_DOUBLE_EQ(FunctionsObject.calcModelValue(0), -100);

}

TEST(OxShmolli2, calcLSResidualsTest) {
    EXPECT_EQ(1, 0);
}

TEST(OxShmolli2, calcLSJacobianTest) {
    EXPECT_EQ(1, 0);
}

TEST(OxShmolli2, calcCostValueTest) {
    EXPECT_EQ(1, 0);
}

TEST(OxShmolli2, calcCostDerivativeTest) {
    EXPECT_EQ(1, 0);
}

