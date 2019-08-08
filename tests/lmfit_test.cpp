/*!
 * \file lmfit_test.cpp
 * \author Konrad Werys
 * \date 2019/08/08
 */

#include "gtest/gtest.h"
#include "lmmin.h"

TEST(lmfit, first_test) {

   std::cout << "aaa" << std::endl;
   lmmin(n, p, n, NULL, NULL, evaluate_nonlin1, &control, &status);
}
