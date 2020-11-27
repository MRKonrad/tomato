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

// correct results has been taken from maltab
// A = [1.47,	1.50,	1.52,	1.55,	1.57,	1.60,	1.63,	1.65,	1.68,	1.70,	1.73,	1.75,	1.78,	1.80,	1.83];                                     % Create Data
// B = [52.21,	53.12,	54.48,	55.84,	57.20,	58.57,	59.93,	61.29,	63.11,	64.47,	66.28,	68.10,	69.92,	72.19,	74.46];
// fitlm(A,B)
TEST(KWUtil, linearFit){

    double datax[] = {1.47,	1.50,	1.52,	1.55,	1.57,	1.60,	1.63,	1.65,	1.68,	1.70,	1.73,	1.75,	1.78,	1.80,	1.83};
    double datay[] = {52.21,	53.12,	54.48,	55.84,	57.20,	58.57,	59.93,	61.29,	63.11,	64.47,	66.28,	68.10,	69.92,	72.19,	74.46};
    int nSamples = 15;

    double correct_a = -39.062;
    double correct_b = 61.272;
    double correct_siga = 2.938;
    double correct_sigb = 1.7759;
    double correct_R2 = 0.9891;
    double correct_chi2 = 7.4906;

    double tolerance = 1e-3;

    double a, b, siga, sigb, R2, chi2;

    KWUtil::linearFit(nSamples, datax, datay, a, b, siga, sigb, R2, chi2);

    EXPECT_NEAR(a, correct_a, tolerance);
    EXPECT_NEAR(b, correct_b, tolerance);
    EXPECT_NEAR(siga, correct_siga, tolerance);
    EXPECT_NEAR(sigb, correct_sigb, tolerance);
    EXPECT_NEAR(R2, correct_R2, tolerance);
    EXPECT_NEAR(chi2, correct_chi2, tolerance);
}

TEST(KWUtil, calculateFitError2x2) {

    double J[4*2] = {0.0811, 0.4359, 0.9294, 0.4468, 0.7757, 0.3063, 0.4868, 0.5085};
    double calculated[2];
    double correct[2] = {1.9444, 2.9508};

    KWUtil::calculateFitError(4, 2, J, 2., calculated);

    EXPECT_NEAR(calculated[0], correct[0], 1e-3);
    EXPECT_NEAR(calculated[1], correct[1], 1e-3);
}

TEST(KWUtil, calculateFitError3x3) {

    double J[7*3] = {
            0.5108, 0.3507, 0.3012,
            0.8176, 0.9390, 0.4709,
            0.7948, 0.8759, 0.2305,
            0.6443, 0.5502, 0.8443,
            0.3786, 0.6225, 0.1948,
            0.8116, 0.5870, 0.2259,
            0.5328, 0.2077, 0.1707
    };
    double calculated[3];
    double correct[3] = {2.0776, 1.9941, 1.7035};

    KWUtil::calculateFitError(7, 3, J, 2., calculated);

    EXPECT_NEAR(calculated[0], correct[0], 1e-3);
    EXPECT_NEAR(calculated[1], correct[1], 1e-3);
    EXPECT_NEAR(calculated[2], correct[2], 1e-3);
}

TEST(KWUtil, calcMatrixInverse2x2) {

    double matrix[4] = {0.8147, 0.9058, 0.1270, 0.9134};
    double invMatrixCorrect[4] = {1.4518, -1.4398, -0.2018, 1.2950};
    double invMatrixCalculated[4];

    EXPECT_EQ(KWUtil::calcMatrixInverse2x2(matrix, invMatrixCalculated), 0); // EXIT_SUCCESS

    EXPECT_NEAR(invMatrixCalculated[0], invMatrixCorrect[0], 1e-3);
    EXPECT_NEAR(invMatrixCalculated[1], invMatrixCorrect[1], 1e-3);
    EXPECT_NEAR(invMatrixCalculated[2], invMatrixCorrect[2], 1e-3);
    EXPECT_NEAR(invMatrixCalculated[3], invMatrixCorrect[3], 1e-3);
}

TEST(KWUtil, calcMatrixInverse2x2_detZero) {

    double matrix[4] = {1, 1, 2, 2};
    double invMatrixCalculated[4];

    EXPECT_EQ(KWUtil::calcMatrixInverse2x2(matrix, invMatrixCalculated), 1); // EXIT_FAILURE

    EXPECT_DOUBLE_EQ(invMatrixCalculated[0], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[1], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[2], 0);
    EXPECT_DOUBLE_EQ(invMatrixCalculated[3], 0);
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

TEST(KWUtil, quickSort) {
    double myarray[5] = {95, -1, -0.1, 3, 1e-2};
    double myarraySorted[5] = {-1, -0.1, 1e-2, 3, 95};

    KWUtil::quickSort(5, myarray);

    EXPECT_DOUBLE_EQ(myarray[0], myarraySorted[0]);
    EXPECT_DOUBLE_EQ(myarray[1], myarraySorted[1]);
    EXPECT_DOUBLE_EQ(myarray[2], myarraySorted[2]);
    EXPECT_DOUBLE_EQ(myarray[3], myarraySorted[3]);
    EXPECT_DOUBLE_EQ(myarray[4], myarraySorted[4]);
}

TEST(KWUtil, calcMeanArray) {

    double myarray[5] = {95, -1, -0.1, 3, 1e-2};

    EXPECT_DOUBLE_EQ(KWUtil::calcMeanArray(5, myarray), 19.382);
}

TEST(KWUtil, calcMedianArray) {

    double myarray[5] = {95, -1, -0.1, 3, 1e-2};

    EXPECT_DOUBLE_EQ(KWUtil::calcMedianArray(5, myarray), 0.01);
}


