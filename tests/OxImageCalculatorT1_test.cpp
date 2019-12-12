/*!
 * \file OxImageCalculatorT1_test.cpp
 * \author Konrad Werys
 * \date 2018/08/07
 */

#include "CmakeConfigForTomato.h"

#include "gtest/gtest.h"
#include "OxTestImage.h"

#include "OxModelT1ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT1WithSignCheck.h"

#include "OxImageCalculatorT1.h"

#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1Shmolli.h"
#include "OxFitterAmoebaPrivateNr2.h"
#include "OxModelT1Shmolli.h"
#include "OxSignCalculatorShmolli.h"
#include "OxStartPointCalculatorShmolli.h"
#endif

#ifdef USE_VNL
TEST(OxImageCalculatorT1, calculate_molli_no_multithread) {

    bool doPrint = false; //for debugging

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/T1_blood.yaml");
    filePaths.push_back("testData/T1_myocardium.yaml");

    int nRows = 10;
    int nCols = 8;

    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nRows, nCols, filePaths);

    int nSamples = testImage->getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> functionsObject;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure calculator
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculatorT1<TYPE> imageCalculator;
    imageCalculator.setCalculatorT1(&calculator);
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
#endif //USE_VNL

#ifdef USE_PRIVATE_NR2
TEST(OxImageCalculatorT1, calculate_shmolli_no_multithread) {

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/T1_blood.yaml");
    filePaths.push_back("testData/T1_myocardium.yaml");

    int nRows = 10;
    int nCols = 8;

    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nRows, nCols, filePaths);

    // init the necessary objects
    Ox::ModelT1Shmolli<TYPE> functionsObject;
    Ox::FitterAmoebaPrivateNr2<TYPE> fitter;
    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    // configure calculator
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculatorT1<TYPE> imageCalculator;
    imageCalculator.setCalculatorT1(&calculator);
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

    TYPE *groundTruthResults = testImage->getImageResultsShmolliPtr();
    std::vector<TYPE> groundTruthResultsVec (groundTruthResults, groundTruthResults + nCols*nRows*3 );
    TYPE *calculaterResults = imageCalculator.getImageResults();
    std::vector<TYPE> calculaterResultsVec (calculaterResults, calculaterResults + nCols*nRows*3 );

    for (int i = 0; i < nCols*nRows*3; ++i) {
        EXPECT_NEAR(groundTruthResultsVec[i], calculaterResultsVec[i], 5e-1); // !!!!!!!!!
    }

    delete testImage;
    delete [] imageResults;
}
#endif //USE_PRIVATE_NR2

#ifndef CXX_STANDARD_98
#ifdef USE_VNL
TEST(OxImageCalculatorT1, calculate_molli_multithread_Vnl) {

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 10;
    int nCols = 8;

    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nRows, nCols, filePaths);

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> functionsObject;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculatorT1<TYPE> imageCalculator;
    imageCalculator.setCalculatorT1(&calculator);
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
#endif //USE_VNL
#endif //CXX_STANDARD_98

#ifndef CXX_STANDARD_98
#ifdef USE_PRIVATE_NR2
TEST(OxImageCalculatorT1, calculate_molli_multithread_nr2) {

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 10;
    int nCols = 8;

    Ox::TestImage<TYPE> *testImage = new Ox::TestImage<TYPE>(nRows, nCols, filePaths);

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> functionsObject;
    Ox::FitterAmoebaPrivateNr2<TYPE> fitter;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&functionsObject);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // image calculator
    Ox::ImageCalculatorT1<TYPE> imageCalculator;
    imageCalculator.setCalculatorT1(&calculator);
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
#endif //USE_PRIVATE_NR2
#endif //CXX_STANDARD_98