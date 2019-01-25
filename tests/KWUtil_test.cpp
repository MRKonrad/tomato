/*!
 * \file KWUtil_test.cpp
 * \author Konrad Werys
 * \date 2018/10/04
 */

#include "KWUtil.h"

#include "gtest/gtest.h"

//#include <iostream>
//#include <cmath>
//#include <cerrno>
//#include <cstring>
//#include <cfenv>
//#pragma STDC FENV_ACCESS ON
//
//TEST(KWUtil, error) {
//
//    if (fpclassify(0) == FP_ZERO) {
//        std::cout << "zero" << std::endl;
//    }
//
//    try {
//        std::cout << exp(710) << std::endl;
//    } catch (...){
//        std::cout << "error" << std::endl;
//    }
//    errno = 0;
//    std::feclearexcept(FE_ALL_EXCEPT);
//    std::cout << "exp(710) = " << std::exp(710) << '\n';
//    if (errno == ERANGE)
//        std::cout << "    errno == ERANGE: " << std::strerror(errno) << '\n';
//    if (std::fetestexcept(FE_OVERFLOW))
//        std::cout << "    FE_OVERFLOW raised\n";
//
////    int a = 1;
////    int b = 0;
////    try{
////        std::cout << "1/0: " << a/b << std::endl;
////    } catch (...) {
////        std::cout << "exception" << std::endl;
////    }
//
////    //fesetenv(3);
////    errno = 0;
////    if (math_errhandling & MATH_ERREXCEPT) feclearexcept(FE_ALL_EXCEPT);
////
////    printf ("Error handling: %d",math_errhandling);
////
////    sqrt (-1);
////    if (math_errhandling & MATH_ERRNO) {
////        if (errno==EDOM) printf("errno set to EDOM\n");
////    }
////    if (math_errhandling  & MATH_ERREXCEPT) {
////        if (fetestexcept(FE_INVALID)) printf("FE_INVALID raised\n");
////    }
//
////    double d = 1.0 / 0.0;
////    switch (fpclassify(d)) {
////        case FP_INFINITE:  printf ("infinite");  break;
////        case FP_NAN:       printf ("NaN");       break;
////        case FP_ZERO:      printf ("zero");      break;
////        case FP_SUBNORMAL: printf ("subnormal"); break;
////        case FP_NORMAL:    printf ("normal");    break;
////    }
////    if (signbit(d)) printf (" negative\n");
////    else printf (" positive or unsigned\n");
//
//}

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


