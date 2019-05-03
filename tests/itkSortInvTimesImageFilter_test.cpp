/*!
 * \file itkSortInvTimesImageFilter_test.cpp
 * \author Konrad Werys
 * \date 2018/08/14
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "OxTestItkImagesFactory.h"
#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorDefault3Dims.h"
#include "OxCalculatorT1Molli.h"

#include "itkCalculatorT1ImageFilter.h"
#include "itkSortInvTimesImageFilter.h"

#include "itkTestingComparisonImageFilter.h"

#include "gtest/gtest.h"

// TODO: diff->GetMaximumDifference() gives an unallocated value. Submit is to ITK.

TEST(itkSortInvTimesImageFilter, sortInvTimesAndMag) {

    typedef double TYPE;
    typedef itk::Image <TYPE, 3> TImageIn;
    typedef itk::Image <TYPE, 2> TImageOut;
    typedef itk::CalculatorT1ImageFilter < TImageIn, TImageOut > ImageCalculatorItkType;
    typedef itk::SortInvTimesImageFilter < TImageIn, TImageIn> SortInvTimesImageFilterType;
    typedef itk::Testing::ComparisonImageFilter<TImageIn, TImageIn> ComparisonImageFilter3dType;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 4;
    int nCols = 5;
    int nSamples = 7;

    // play with the order
    std::vector< int > invTimesOrder(nSamples);
    invTimesOrder.at(0) = 0;
    invTimesOrder.at(1) = 5;
    invTimesOrder.at(2) = 6;
    invTimesOrder.at(3) = 1;
    invTimesOrder.at(4) = 2;
    invTimesOrder.at(5) = 3;
    invTimesOrder.at(6) = 4;

    // test images
    Ox::TestItkImagesFactory<TYPE> sortedImagesFactory(nRows, nCols, filePaths);
    Ox::TestItkImagesFactory<TYPE> notSortedImagesFactory(nRows, nCols, filePaths, invTimesOrder);

    TImageIn::Pointer imageMagNotSorted = notSortedImagesFactory.generateImageMag();
    TImageIn::Pointer imageMagSorted = sortedImagesFactory.generateImageMag();;

    SortInvTimesImageFilterType::Pointer sortFilter = SortInvTimesImageFilterType::New();
    sortFilter->SetInput(imageMagNotSorted);
    sortFilter->SetInvTimesNonSorted(notSortedImagesFactory.getInvTimesVnl());
    sortFilter->Update();

    // for comparing invTimes
    std::vector< TYPE > sortedInvTimes(nSamples);
    sortFilter->GetInvTimesSorted().copy_out(&sortedInvTimes.front());

    // for comparing the images
    ComparisonImageFilter3dType::Pointer diff = ComparisonImageFilter3dType::New();

    diff->SetValidInput(imageMagSorted.GetPointer());
    diff->SetTestInput(sortFilter->GetOutput());
    diff->UpdateLargestPossibleRegion();

    // TODO: add
    //ASSERT_EQ(sortedImagesFactory.getInvTimes(), sortedInvTimes);
    ASSERT_EQ(diff->GetNumberOfPixelsWithDifferences(), 0);
}

#endif // USE_ITK