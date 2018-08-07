/*!
 * \file OxImageCalculator_test.cpp
 * \author Konrad Werys
 * \date 2018/08/07
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "OxImageCalculator.h"

TEST(OxImageCalculator, doAnything) {

    typedef double TYPE;

    Ox::ImageCalculator<TYPE> imageCalculator;
    imageCalculator.calculateThreaded();

    EXPECT_EQ(1 , 1);
}