/*!
 * \file OxImageCalculator_test.cpp
 * \author Konrad Werys
 * \date 2018/08/07
 */

#include "gtest/gtest.h"
#include "OxTestImage.h"

#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorRealImag.h"
#include "OxCalculatorT1Molli.h"

#include "OxImageCalculator.h"

TEST(OxImageCalculator, calculate_no_multithread) {

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nCols = 10;
    int nRows = 8;
    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nCols, nRows, filePaths);

//    int nSamples = testImage->getNSamples();
//
//    std::cout << std::endl;
//    std::cout << std::endl;
//    for (int i = 0; i < nRows*nCols*nSamples; ++i){
//        std::cout << " " << testImage->getImageMag()[i];
//    }
//    std::cout << std::endl;
//
//
//    std::cout << std::endl;
//    std::cout << std::endl;
//    for (int iSample = 0; iSample < nSamples; ++iSample) {
//        for (int iRow = 0; iRow < nRows; ++iRow) {
//            for (int iCol = 0; iCol < nCols; ++iCol) {
//                int i = iSample * (nCols * nRows) + iRow * nCols + iCol;
//                std::cout << " " << testImage->getImageMag()[i];
//            }
//            std::cout << std::endl;
//        }
//        std::cout << std::endl;
//        std::cout << std::endl;
//    }

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculator<TYPE> signCalculator;
    Ox::StartPointCalculator<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculator<TYPE> imageCalculator;
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

//    int nDims = 3;
//    std::cout << std::endl;
//    std::cout << std::endl;
//    for (int iDim = 0; iDim < nDims; ++iDim) {
//        for (int iRow = 0; iRow < nRows; ++iRow) {
//            for (int iCol = 0; iCol < nCols; ++iCol) {
//                int i = iDim * (nCols * nRows) + iRow * nCols + iCol;
//                std::cout << " " << imageCalculator.getImageResults()[i];
//            }
//            std::cout << std::endl;
//        }
//        std::cout << std::endl;
//        std::cout << std::endl;
//    }

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

TEST(OxImageCalculator, playing_with_clonning) {

//    typedef double TYPE;
//
//    std::vector< std::string > filePaths;
//    filePaths.push_back("testData/blood.yaml");
//    filePaths.push_back("testData/myocardium.yaml");
//
//    int nCols = 2;
//    int nRows = 1;
//    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nCols, nRows, filePaths);
//
//    TYPE test[7] = {1, 2, 3, 4, 5, 6, 7};
//
//    // init the necessary objects
//    Ox::FunctionsT1Basic<TYPE> functionsObject;
//    functionsObject.setNSamples(testImage->getNSamples());
//    functionsObject.setInvTimes(testImage->getInvTimesPtr());
//    functionsObject.setSignal(test);
//    functionsObject.disp();
//    std::cout << functionsObject.getInvTimes() << std::endl;
//
//    TYPE test2[7] = {2, 2, 3, 4, 5, 6, 7};
//    Ox::FunctionsT1Basic<TYPE> functionsObjectCopy = functionsObject;
//    functionsObjectCopy.setSignal(test2);
//    functionsObjectCopy.disp();
//    std::cout << functionsObjectCopy.getInvTimes() << std::endl;
//
//    TYPE test3[7] = {3, 2, 3, 4, 5, 6, 7};
//    Ox::FunctionsT1Basic<TYPE> functionsObjectCopy2(functionsObject);
//    functionsObjectCopy2.setSignal(test3);
//    functionsObjectCopy2.disp();
//    std::cout << functionsObjectCopy2.getInvTimes() << std::endl;

//    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
//    Ox::SignCalculator<TYPE> signCalculator;
//    Ox::StartPointCalculator<TYPE> startPointCalculator;
//    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;
//
//    // configure
//    calculatorT1Molli.setFunctionsT1(&functionsObject);
//    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
//    calculatorT1Molli.setSignCalculator(&signCalculator);
//    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
//
//    // image calculator
//    Ox::ImageCalculator<TYPE> imageCalculator;
//    imageCalculator.setCalculatorT1(&calculatorT1Molli);
//    imageCalculator.setInvTimes(testImage->getInvTimesPtr());
//    imageCalculator.setImageMag(testImage->getImageMagPtr());
//    imageCalculator.setImagePha(testImage->getImagePhaPtr());
//    imageCalculator.setNCols(nCols);
//    imageCalculator.setNRows(nRows);
//    imageCalculator.setNSamples(testImage->getNSamples());

}