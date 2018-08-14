/*!
 * \file itkOxT1ImageFilter_test.cpp
 * \author Konrad Werys
 * \date 2018/08/13
 */


#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

#include "OxTestItkImagesFactory.h"
#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include <OxSignCalculatorNoSign.h>
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorDefault3Dims.h"
#include "OxCalculatorT1Molli.h"

#include "itkCalculatorT1ImageFilter.h"
#include "itkTestingComparisonImageFilter.h"

#include "gtest/gtest.h"

TEST(CalculatorT1ImageFilter, calculate) {

    typedef double TYPE;
    typedef itk::Image <TYPE, 3> TImageIn;
    typedef itk::Image <TYPE, 2> TImageOut;
    typedef itk::CalculatorT1ImageFilter < TImageIn, TImageOut > ImageCalculatorItkType;
    typedef itk::Testing::ComparisonImageFilter<TImageIn, TImageIn> ComparisonImageFilter3dType;
    typedef itk::Testing::ComparisonImageFilter<TImageOut, TImageOut> ComparisonImageFilter2dType;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 2;
    int nCols = 3;

    // test images
    Ox::TestItkImagesFactory<TYPE> itkImagesFactory(nRows, nCols, filePaths);

    TImageIn::Pointer imageMag = itkImagesFactory.gererateImageMag();
    TImageIn::Pointer imagePha = itkImagesFactory.gererateImagePha();

    TImageOut::Pointer imageMolliA = itkImagesFactory.gererateImageResultsMolliA();
    TImageOut::Pointer imageMolliB = itkImagesFactory.gererateImageResultsMolliB();
    TImageOut::Pointer imageMolliT1star = itkImagesFactory.gererateImageResultsMolliT1star();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure calculator
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
    calculatorT1Molli.setInvTimes(itkImagesFactory.getInvTimesPtr());
    calculatorT1Molli.setNSamples(itkImagesFactory.getNSamples());

    // configure calculator itk filter
    ImageCalculatorItkType::Pointer imageCalculatorItk = ImageCalculatorItkType::New();
    imageCalculatorItk->SetInputMagImage(imageMag);
    imageCalculatorItk->SetInputPhaImage(imagePha);
    imageCalculatorItk->SetCalculator(&calculatorT1Molli);
    //imageCalculatorItk->SetNumberOfThreads(1);
    imageCalculatorItk->Update();

    ComparisonImageFilter2dType::Pointer diff = ComparisonImageFilter2dType::New();

    diff->SetValidInput(imageMolliA.GetPointer());
    diff->SetTestInput(imageCalculatorItk->GetAImage());
    diff->UpdateLargestPossibleRegion();

    EXPECT_LE(diff->GetMaximumDifference(), 1e-2);

    diff->SetValidInput(imageMolliB.GetPointer());
    diff->SetTestInput(imageCalculatorItk->GetBImage());
    diff->UpdateLargestPossibleRegion();

    EXPECT_LE(diff->GetMaximumDifference(), 1e-2);

    diff->SetValidInput(imageMolliT1star.GetPointer());
    diff->SetTestInput(imageCalculatorItk->GetT1starImage());
    diff->UpdateLargestPossibleRegion();

    EXPECT_LE(diff->GetMaximumDifference(), 1e-2);

}

TEST(CalculatorT1ImageFilter, calculate_no_phase_given) {
    typedef double TYPE;
    typedef itk::Image <TYPE, 3> TImageIn;
    typedef itk::Image <TYPE, 2> TImageOut;
    typedef itk::CalculatorT1ImageFilter < TImageIn, TImageOut > ImageCalculatorItkType;
    typedef itk::Testing::ComparisonImageFilter<TImageIn, TImageIn> ComparisonImageFilter3dType;
    typedef itk::Testing::ComparisonImageFilter<TImageOut, TImageOut> ComparisonImageFilter2dType;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 4;
    int nCols = 5;

    // test images
    Ox::TestItkImagesFactory<TYPE> itkImagesFactory(nRows, nCols, filePaths);

    TImageIn::Pointer imageMag = itkImagesFactory.gererateImageMag();
    //TImageIn::Pointer imagePha = itkImagesFactory.gererateImagePha();

    TImageOut::Pointer imageMolliA = itkImagesFactory.gererateImageResultsMolliA();
    TImageOut::Pointer imageMolliB = itkImagesFactory.gererateImageResultsMolliB();
    TImageOut::Pointer imageMolliT1star = itkImagesFactory.gererateImageResultsMolliT1star();

    // init the necessary objects
    Ox::FunctionsT1Basic<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure calculator
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
    calculatorT1Molli.setInvTimes(itkImagesFactory.getInvTimesPtr());
    calculatorT1Molli.setNSamples(itkImagesFactory.getNSamples());

    // configure calculator itk filter
    ImageCalculatorItkType::Pointer imageCalculatorItk = ImageCalculatorItkType::New();
    imageCalculatorItk->SetInputMagImage(imageMag);
    // imageCalculatorItk->SetInputPhaImage(imagePha);
    imageCalculatorItk->SetCalculator(&calculatorT1Molli);
    //imageCalculatorItk->SetNumberOfThreads(1);
    imageCalculatorItk->Update();

    ComparisonImageFilter2dType::Pointer diff = ComparisonImageFilter2dType::New();

    diff->SetValidInput(imageMolliA.GetPointer());
    diff->SetTestInput(imageCalculatorItk->GetAImage());
    diff->UpdateLargestPossibleRegion();

    EXPECT_LE(diff->GetMaximumDifference(), 1e-2);

    diff->SetValidInput(imageMolliB.GetPointer());
    diff->SetTestInput(imageCalculatorItk->GetBImage());
    diff->UpdateLargestPossibleRegion();

    EXPECT_LE(diff->GetMaximumDifference(), 1e-2);

    diff->SetValidInput(imageMolliT1star.GetPointer());
    diff->SetTestInput(imageCalculatorItk->GetT1starImage());
    diff->UpdateLargestPossibleRegion();

    EXPECT_LE(diff->GetMaximumDifference(), 1e-2);
}

#endif //USE_ITK

