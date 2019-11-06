/*!
 * \file itkOxT1ImageFilter_test.cpp
 * \author Konrad Werys
 * \date 2018/08/13
 */


#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "OxTestItkImagesFactory.h"
#include "OxFunctionsT1ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT1Molli.h"

#include "itkCalculatorT1ImageFilter.h"

#include "itkTestingComparisonImageFilter.h"

#include "gtest/gtest.h"

TEST(itkCalculatorT1ImageFilter, calculate) {

    typedef double TYPE;
    typedef itk::Image <TYPE, 3> TImageIn;
    typedef itk::Image <TYPE, 2> TImageOut;
    typedef itk::CalculatorT1ImageFilter < TImageIn, TImageOut > CalculatorT1ImageFilterType;
    typedef itk::Testing::ComparisonImageFilter<TImageIn, TImageIn> ComparisonImageFilter3dType;
    typedef itk::Testing::ComparisonImageFilter<TImageOut, TImageOut> ComparisonImageFilter2dType;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/T1_blood.yaml");
    filePaths.push_back("testData/T1_myocardium.yaml");

    int nRows = 2;
    int nCols = 3;

    // test images
    Ox::TestItkImagesFactory<TYPE> itkImagesFactory(nRows, nCols, filePaths);

    TImageIn::Pointer imageMag = itkImagesFactory.generateImageMag();
    TImageIn::Pointer imagePha = itkImagesFactory.generateImagePha();

    TImageOut::Pointer imageMolliA = itkImagesFactory.generateImageResultsMolliA();
    TImageOut::Pointer imageMolliB = itkImagesFactory.generateImageResultsMolliB();
    TImageOut::Pointer imageMolliT1star = itkImagesFactory.generateImageResultsMolliT1star();

    // init the necessary objects
    Ox::FunctionsT1ThreeParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1Molli<TYPE> calculatorT1Molli;

    // configure calculator
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
    calculatorT1Molli.setInvTimes(itkImagesFactory.getInvTimesPtr());
    calculatorT1Molli.setNSamples(itkImagesFactory.getNSamples());

    // configure calculator itk filter
    CalculatorT1ImageFilterType::Pointer imageCalculatorItk = CalculatorT1ImageFilterType::New();
    imageCalculatorItk->SetInputMagImage(imageMag);
    imageCalculatorItk->SetInputPhaImage(imagePha);
    imageCalculatorItk->SetCalculator(&calculatorT1Molli);
    imageCalculatorItk->SetLowerLimitOutputIntensity(0);
    imageCalculatorItk->SetUpperLimitOutputIntensity(4000);

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

TEST(itkCalculatorT1ImageFilter, calculate_no_phase_given) {

    typedef double TYPE;
    typedef itk::Image <TYPE, 3> TImageIn;
    typedef itk::Image <TYPE, 2> TImageOut;
    typedef itk::CalculatorT1ImageFilter < TImageIn, TImageOut > ImageCalculatorItkType;
    typedef itk::Testing::ComparisonImageFilter<TImageIn, TImageIn> ComparisonImageFilter3dType;
    typedef itk::Testing::ComparisonImageFilter<TImageOut, TImageOut> ComparisonImageFilter2dType;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/T1_blood.yaml");
    filePaths.push_back("testData/T1_myocardium.yaml");

    int nRows = 4;
    int nCols = 5;

    // test images
    Ox::TestItkImagesFactory<TYPE> itkImagesFactory(nRows, nCols, filePaths);

    TImageIn::Pointer imageMag = itkImagesFactory.generateImageMag();
    //TImageIn::Pointer imagePha = itkImagesFactory.generateImagePha();

    TImageOut::Pointer imageMolliA = itkImagesFactory.generateImageResultsMolliA();
    TImageOut::Pointer imageMolliB = itkImagesFactory.generateImageResultsMolliB();
    TImageOut::Pointer imageMolliT1star = itkImagesFactory.generateImageResultsMolliT1star();

    // init the necessary objects
    Ox::FunctionsT1ThreeParam<TYPE> functionsObject;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
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

