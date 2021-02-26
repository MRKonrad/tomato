/*!
 * \file OxImageCalculatorT2_test.cpp
 * \author Konrad Werys
 * \date 2020/12/09
 */

#include "CmakeConfigForTomato.h"

#ifdef USE_PRIVATE_NR2

#include "gtest/gtest.h"

#include "OxImageCalculator.h"

#include "OxCalculatorT2.h"
#include "OxModelT2TwoParamScale.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxCalculatorT2Linear.h"

#ifdef USE_VNL
TEST(OxImageCalculatorT2, calculateNonLinear) {

    bool useThreads = true;
#ifdef TOMATO_USES_CXX_STANDARD_98
    useThreads = false;
#endif

    typedef double TYPE;

    int nRows = 5;
    int nCols = 1;
    int nSamples = 8;

    // input data
    TYPE magData[] = {
            110, 110, 110, 110, 110,
             75,  75,  75,  75,  75,
             49,  49,  49,  49,  49,
             35,  35,  35,  35,  35,
             25,  25,  25,  25,  25,
             18,  18,  18,  18,  18,
             11,  11,  11,  11,  11,
              8,   8,   8,   8,   8
    };
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};

    // correct values
    TYPE tolerance = 1e-4;
    std::map <std::string, std::vector<TYPE> > correctMap;
    correctMap["B"] = std::vector<TYPE>(nCols * nRows, 159.9788);
    correctMap["T2"] = std::vector<TYPE>(nCols * nRows, 7.9055);
    correctMap["R2"] = std::vector<TYPE>(nCols * nRows, 0.9990);
    correctMap["deltaB"] = std::vector<TYPE>(nCols * nRows, 2.3782);
    correctMap["deltaT2"] = std::vector<TYPE>(nCols * nRows, 0.1389);

    // init the necessary objects
    Ox::ModelT2TwoParamScale<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::CalculatorT2<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitter);

    // image calculator
    Ox::ImageCalculator<TYPE> imageCalculator;
    imageCalculator.setCalculator(&calculator);
    imageCalculator.setEchoTimes(echoTimes);
    imageCalculator.setImageMag(magData);
    imageCalculator.setNCols(nCols);
    imageCalculator.setNRows(nRows);
    imageCalculator.setNSamples(nSamples);
    imageCalculator.setUseThreads(useThreads);
    imageCalculator.setNThreads(3);

    // results array have to be allocated and calculated
    std::map <std::string, TYPE *> resultsMap;
    resultsMap["B"] = new TYPE[nCols*nRows];
    resultsMap["T2"] = new TYPE[nCols*nRows];
    resultsMap["R2"] = new TYPE[nCols*nRows];
    resultsMap["deltaB"] = new TYPE[nCols*nRows];
    resultsMap["deltaT2"] = new TYPE[nCols*nRows];
    imageCalculator.setImageResultsMap(&resultsMap);

    // calculate
    imageCalculator.calculate();

    // compare
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["B"].data(), resultsMap["B"], tolerance, "B"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["T2"].data(), resultsMap["T2"], tolerance, "T2"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["R2"].data(), resultsMap["R2"], tolerance, "R2"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaB"].data(), resultsMap["deltaB"], tolerance, "deltaB"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaT2"].data(), resultsMap["deltaT2"], tolerance, "deltaT2"));

    // clean up
    delete [] resultsMap["B"];
    delete [] resultsMap["T2"];
    delete [] resultsMap["R2"];
    delete [] resultsMap["deltaB"];
    delete [] resultsMap["deltaT2"];
}
#endif // USE_VNL

TEST(OxImageCalculatorT2, calculateLinear) {

    bool useThreads = true;
#ifdef TOMATO_USES_CXX_STANDARD_98
    useThreads = false;
#endif

    typedef double TYPE;

    int nRows = 5;
    int nCols = 1;
    int nSamples = 8;

    // input data
    TYPE magData[] = {
            110, 110, 110, 110, 110,
            75,  75,  75,  75,  75,
            49,  49,  49,  49,  49,
            35,  35,  35,  35,  35,
            25,  25,  25,  25,  25,
            18,  18,  18,  18,  18,
            11,  11,  11,  11,  11,
            8,   8,   8,   8,   8
    };
    TYPE echoTimes[] = {3, 6, 9, 12, 15, 18, 21, 24};

    // correct values
    TYPE tolerance = 1e-4;
    std::map <std::string, std::vector<TYPE> > correctMap;
    correctMap["B"] = std::vector<TYPE>(nCols * nRows, 157.1976);
    correctMap["T2"] = std::vector<TYPE>(nCols * nRows, 8.0547);
    correctMap["R2"] = std::vector<TYPE>(nCols * nRows, 0.9982);
    correctMap["deltaB"] = std::vector<TYPE>(nCols * nRows, 5.1552);
    correctMap["deltaT2"] = std::vector<TYPE>(nCols * nRows, 0.1404);

    // init the necessary objects
    Ox::CalculatorT2Linear<TYPE> calculator;

    // image calculator
    Ox::ImageCalculator<TYPE> imageCalculator;
    imageCalculator.setCalculator(&calculator);
    imageCalculator.setEchoTimes(echoTimes);
    imageCalculator.setImageMag(magData);
    imageCalculator.setNCols(nCols);
    imageCalculator.setNRows(nRows);
    imageCalculator.setNSamples(nSamples);
    imageCalculator.setUseThreads(useThreads);
    imageCalculator.setNThreads(3);

    // results array have to be allocated and calculated
    std::map <std::string, TYPE *> resultsMap;
    resultsMap["B"] = new TYPE[nCols*nRows];
    resultsMap["T2"] = new TYPE[nCols*nRows];
    resultsMap["R2"] = new TYPE[nCols*nRows];
    resultsMap["deltaB"] = new TYPE[nCols*nRows];
    resultsMap["deltaT2"] = new TYPE[nCols*nRows];
    imageCalculator.setImageResultsMap(&resultsMap);

    // calculate
    imageCalculator.calculate();

    // compare
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["B"].data(), resultsMap["B"], tolerance, "B"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["T2"].data(), resultsMap["T2"], tolerance, "T2"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["R2"].data(), resultsMap["R2"], tolerance, "R2"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaB"].data(), resultsMap["deltaB"], tolerance, "deltaB"));
    EXPECT_TRUE(KWUtil::array_expect_near(nCols*nRows, correctMap["deltaT2"].data(), resultsMap["deltaT2"], tolerance, "deltaT2"));

    // clean up
    delete [] resultsMap["B"];
    delete [] resultsMap["T2"];
    delete [] resultsMap["R2"];
    delete [] resultsMap["deltaB"];
    delete [] resultsMap["deltaT2"];
}

#endif // USE_PRIVATE_NR2