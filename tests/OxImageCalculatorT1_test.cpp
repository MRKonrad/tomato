/*!
 * \file OxImageCalculatorT1_test.cpp
 * \author Konrad Werys
 * \date 2020/12/07
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"

#include "OxImageCalculator.h"

#include "OxModelT1ThreeParam.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT1WithSignCheck.h"

#ifdef USE_VNL
TEST(OxImageCalculatorT1, calculate_molli) {

    bool useThreads = true;
#ifdef TOMATO_USES_CXX_STANDARD_98
    useThreads = false;
#endif

    typedef double TYPE;

    int nRows = 5;
    int nCols = 1;
    int nSamples = 7;

    // input data
    TYPE magData[] = {
            -55, -55, -55, -55, -55,
            -49, -49, -49, -49, -49,
            -29, -29, -29, -29, -29,
             22,  22,  22,  22,  22,
             51,  51,  51,  51,  51,
             61,  61,  61,  61,  61,
             64,  64,  64,  64,  64,
    };
    TYPE invTimes[] = {100,  180,  260, 1715, 3337, 4907, 6455};

    // correct values
    TYPE tolerance = 1e-4;
    std::map <std::string, std::vector<TYPE> > correctMap;
    correctMap["A"] = std::vector<TYPE>(nCols * nRows, 65.8967);
    correctMap["B"] = std::vector<TYPE>(nCols * nRows, 124.079);
    correctMap["T1star"] = std::vector<TYPE>(nCols * nRows, 1571.8001);
    correctMap["T1"] = std::vector<TYPE>(nCols * nRows, 1387.7952);
    correctMap["R2Abs"] = std::vector<TYPE>(nCols * nRows, 0.9165);
    correctMap["deltaA"] = std::vector<TYPE>(nCols * nRows, 5.8696);
    correctMap["deltaB"] = std::vector<TYPE>(nCols * nRows, 6.2143);
    correctMap["deltaT1star"] = std::vector<TYPE>(nCols * nRows, 298.2294);
    correctMap["deltaT1"] = std::vector<TYPE>(nCols * nRows, 675.1654);

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure calculator
    calculator.setModel(&model);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculator<TYPE> imageCalculator;
    imageCalculator.setCalculator(&calculator);
    imageCalculator.setInvTimes(invTimes);
    imageCalculator.setImageMag(magData);
    imageCalculator.setNCols(nCols);
    imageCalculator.setNRows(nRows);
    imageCalculator.setNSamples(nSamples);
    imageCalculator.setUseThreads(useThreads);
    imageCalculator.setNThreads(3);

    // results array have to be allocated and calculated
    std::map <std::string, TYPE *> resultsMap;
    resultsMap["A"] = new TYPE[nCols*nRows];
    resultsMap["B"] = new TYPE[nCols*nRows];
    resultsMap["T1star"] = new TYPE[nCols*nRows];
    resultsMap["T1"] = new TYPE[nCols*nRows];
    resultsMap["R2Abs"] = new TYPE[nCols*nRows];
    resultsMap["deltaA"] = new TYPE[nCols*nRows];
    resultsMap["deltaB"] = new TYPE[nCols*nRows];
    resultsMap["deltaT1star"] = new TYPE[nCols*nRows];
    resultsMap["deltaT1"] = new TYPE[nCols*nRows];
    imageCalculator.setImageResultsMap(&resultsMap);

    // calculate
    imageCalculator.calculate();

    // compare
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["A"].data(), resultsMap["A"], tolerance, "A"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["B"].data(), resultsMap["B"], tolerance, "B"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["T1star"].data(), resultsMap["T1star"], tolerance, "T1star"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["T1"].data(), resultsMap["T1"], tolerance, "T1"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["R2Abs"].data(), resultsMap["R2Abs"], tolerance, "R2"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaA"].data(), resultsMap["deltaA"], tolerance, "deltaA"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaB"].data(), resultsMap["deltaB"], tolerance, "deltaB"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaT1star"].data(), resultsMap["deltaT1star"], tolerance, "deltaT1star"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaT1"].data(), resultsMap["deltaT1"], tolerance, "deltaT1"));

    // clean up
    delete [] resultsMap["A"];
    delete [] resultsMap["B"];
    delete [] resultsMap["T1star"];
    delete [] resultsMap["T1"];
    delete [] resultsMap["R2Abs"];
    delete [] resultsMap["deltaA"];
    delete [] resultsMap["deltaB"];
    delete [] resultsMap["deltaT1star"];
    delete [] resultsMap["deltaT1"];
}
#endif // USE_VNL