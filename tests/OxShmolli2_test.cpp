/*!
 * \file OxShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

#include "gtest/gtest.h"

#include "OxShmolli2.h"

TEST(OxShmolli2, readAndSort_calculate) {

    Ox::OxShmolli2 OxShmolli2_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
    EXPECT_EQ(OxShmolli2_object.readAndSort(), 0); // EXIT_SUCCESS
    EXPECT_EQ(OxShmolli2_object.calculate(), 0); // EXIT_SUCCESS

}

#endif // USE_ITK