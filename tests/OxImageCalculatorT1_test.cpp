/*!
 * \file OxImageCalculatorT1_test.cpp
 * \author Konrad Werys
 * \date 2018/08/07
 */

#include "gtest/gtest.h"
#include "OxTestImage.h"

#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorDefault3Dims.h"
#include "OxCalculatorT1Molli.h"

#include "OxImageCalculatorT1.h"

TEST(OxImageCalculatorT1, calculate_no_multithread) {

    bool doPrint = false; //for debugging

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 10;
    int nCols = 8;

    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nRows, nCols, filePaths);

    int nSamples = testImage->getNSamples();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure calculator
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculatorT1<TYPE> imageCalculator;
    imageCalculator.setCalculatorT1(&calculatorT1Molli);
    imageCalculator.setInvTimes(testImage->getInvTimesPtr());
    imageCalculator.setImageMag(testImage->getImageMagPtr());
    imageCalculator.setImagePha(testImage->getImagePhaPtr());
    imageCalculator.setNCols(nCols);
    imageCalculator.setNRows(nRows);
    imageCalculator.setNSamples(testImage->getNSamples());
    imageCalculator.setUseThreads(false);

    // alloc results
    TYPE *imageResults = new TYPE[nCols*nRows*3];
    imageCalculator.setImageResults(imageResults);

    // calculate
    imageCalculator.calculate();

    TYPE *groundTruthResults = testImage->getImageResultsMolliPtr();
    std::vector<TYPE> groundTruthResultsVec (groundTruthResults, groundTruthResults + nCols*nRows*3 );
    TYPE *calculaterResults = imageCalculator.getImageResults();
    std::vector<TYPE> calculaterResultsVec (calculaterResults, calculaterResults + nCols*nRows*3 );

    for (int i = 0; i < nCols*nRows*3; ++i) {
        EXPECT_NEAR(groundTruthResultsVec[i], calculaterResultsVec[i], 1e-2);
    }

    if(doPrint){
        std::cout << std::endl;
        std::cout << std::endl;
        for (int i = 0; i < nRows*nCols*nSamples; ++i){
            std::cout << " " << testImage->getImageMagPtr()[i];
        }
        std::cout << std::endl;


        std::cout << std::endl;
        std::cout << std::endl;
        for (int iSample = 0; iSample < nSamples; ++iSample) {
            for (int iRow = 0; iRow < nRows; ++iRow) {
                for (int iCol = 0; iCol < nCols; ++iCol) {
                    int i = iSample * (nCols * nRows) + iRow * nCols + iCol;
                    if (iRow == 2 && iCol == 1) {
                        std::cout << " xx";
                    } else {
                        //std::cout << " " << testImage->getImageMagPtr()[i];
                        std::cout << " " << testImage->getImagePhaPtr()[i];
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }

        int nDims = 3;
        std::cout << std::endl;
        std::cout << std::endl;
        for (int iDim = 0; iDim < nDims; ++iDim) {
            for (int iRow = 0; iRow < nRows; ++iRow) {
                for (int iCol = 0; iCol < nCols; ++iCol) {
                    int i = iDim * (nCols * nRows) + iRow * nCols + iCol;
                    std::cout << " " << imageCalculator.getImageResults()[i];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    delete testImage;
    delete [] imageResults;
}

#ifndef CXX_STANDARD_98

TEST(OxImageCalculatorT1, calculate_multithread) {

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 8;
    int nCols = 10;

    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nRows, nCols, filePaths);

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculator<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculatorT1<TYPE> imageCalculator;
    imageCalculator.setCalculatorT1(&calculatorT1Molli);
    imageCalculator.setInvTimes(testImage->getInvTimesPtr());
    imageCalculator.setImageMag(testImage->getImageMagPtr());
    imageCalculator.setImagePha(testImage->getImagePhaPtr());
    imageCalculator.setNCols(nCols);
    imageCalculator.setNRows(nRows);
    imageCalculator.setNSamples(testImage->getNSamples());

    // alloc results
    TYPE *imageResults = new TYPE[nCols*nRows*3];
    imageCalculator.setImageResults(imageResults);

    // calculate
    imageCalculator.calculate();

    TYPE *groundTruthResults = testImage->getImageResultsMolliPtr();
    std::vector<TYPE> groundTruthResultsVec (groundTruthResults, groundTruthResults + nCols*nRows*3 );
    TYPE *calculaterResults = imageCalculator.getImageResults();
    std::vector<TYPE> calculaterResultsVec (calculaterResults, calculaterResults + nCols*nRows*3 );

    for (int i = 0; i < nCols*nRows*3; ++i) {
        EXPECT_NEAR(groundTruthResultsVec[i], calculaterResultsVec[i], 1e-2);
    }

    delete testImage;
    delete [] imageResults;
}

#endif