/*!
 * \file TomatoOptions_test.cpp
 * \author Konrad Werys
 * \date 2021/07/05
 */

#include "CmakeConfigForTomato.h"
#include "gtest/gtest.h"

#ifdef USE_YAML

#include "TomatoOptions.h"

TEST(TomatoOptions, printCurrent) {
    Ox::TomatoOptions<double> tomatoOptions;
    EXPECT_NO_THROW(tomatoOptions.printCurrent());
}

TEST(TomatoOptions, exportT) {
    Ox::TomatoOptions<double> tomatoOptions;
    tomatoOptions.dir_output_map = ".";
    EXPECT_NO_THROW(tomatoOptions.exportToYaml());
}

#endif // USE_YAML