/*!
 * \file itkPipeline_CalculatorShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/10/12
 */


#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "itkReadFileListFilter.h"
#include "itkSortInvTimesImageFilter.h"
#include "itkCalculatorT1ImageFilter.h"
#include "itkColorbar2DImageFilter.h"
#include "itkTestingComparisonImageFilter.h"

#ifdef USE_PRIVATE_NR2
#include "OxFitterAmoebaPrivateNr2.h"
#include "OxCalculatorT1Shmolli.h"
#include "OxFunctionsT1Shmolli.h"
#include "OxSignCalculatorShmolli.h"
#include "OxStartPointCalculatorShmolli.h"

#include "gtest/gtest.h"


TEST(itkPipeline_CalculatorShmolli, calculateShmolliAndCompareWithDicom) {

    // primitive data types
    typedef double InputPixelType;
    typedef double OutputPixelType;

    // image data types
    typedef itk::Image< InputPixelType,  3 > ImageType3D;
    typedef itk::Image< OutputPixelType, 2 > ImageType2D;
    typedef itk::ImageFileReader<ImageType2D> ReadImage2dFilterType;
    typedef itk::ReadFileListFilter<ImageType3D> ReadFileList3dFilterType;
    typedef itk::SortInvTimesImageFilter < ImageType3D, ImageType3D> SortInvTimesImageFilterType;
    typedef itk::CalculatorT1ImageFilter < ImageType3D, ImageType2D > CalculatorT1ImageFilterType;
    typedef itk::Colorbar2DImageFilter <ImageType2D> Colorbar2DImageFilterType;
    typedef itk::Testing::ComparisonImageFilter<ImageType2D, ImageType2D> ComparisonImageFilter2dType;

    std::vector<std::string> dcmMagFilePaths;
    std::vector<std::string> dcmPhaFilePaths;
    std::vector<std::string> dcmT1MapFilePaths;

    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0001.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0002.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0003.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0004.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0005.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0006.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0007.dcm");

    dcmPhaFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13/IM-0002-0001.dcm");
    dcmPhaFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13/IM-0002-0002.dcm");
    dcmPhaFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13/IM-0002-0003.dcm");
    dcmPhaFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13/IM-0002-0004.dcm");
    dcmPhaFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13/IM-0002-0005.dcm");
    dcmPhaFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13/IM-0002-0006.dcm");
    dcmPhaFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13/IM-0002-0007.dcm");

    dcmT1MapFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_T1MAP_14/IM-0003-0001.dcm");

    ReadFileList3dFilterType::Pointer readerMag = ReadFileList3dFilterType::New();
    readerMag->SetFileList(dcmMagFilePaths);
    readerMag->Update();

    ReadFileList3dFilterType::Pointer readerPha = ReadFileList3dFilterType::New();
    readerPha->SetFileList(dcmPhaFilePaths);
    readerPha->Update();

    ReadImage2dFilterType::Pointer readerT1Map = ReadImage2dFilterType::New();
    readerT1Map->SetFileName(dcmT1MapFilePaths.at(0));
    readerT1Map->Update();

    SortInvTimesImageFilterType::Pointer sorterMag = SortInvTimesImageFilterType::New();
    sorterMag->SetInvTimesNonSorted(readerMag->GetInvTimes());
    sorterMag->SetInput(readerMag->GetOutput());
    sorterMag->Update();

    SortInvTimesImageFilterType::Pointer sorterPha = SortInvTimesImageFilterType::New();
    sorterPha->SetInvTimesNonSorted(readerPha->GetInvTimes());
    sorterPha->SetInput(readerPha->GetOutput());
    sorterPha->Update();

    // if I do not store a copy of inversion times like this, vnl_vector does funny things with the data under
    // the pointer .data_block();
    int nSamples = sorterMag->GetInvTimesSorted().size();
    InputPixelType *invTimes = new InputPixelType[nSamples];
    KWUtil::copyArrayToArray(nSamples, invTimes, sorterMag->GetInvTimesSorted().data_block());

    // init the necessary objects
    Ox::FunctionsT1Shmolli<InputPixelType> functionsObject;
    Ox::FitterAmoebaPrivateNr2<InputPixelType> fitter;
    Ox::SignCalculatorShmolli<InputPixelType> signCalculator;
    Ox::StartPointCalculatorShmolli<InputPixelType> startPointCalculator;
    Ox::CalculatorT1Shmolli<InputPixelType> calculatorT1;

    // configure calculator
    calculatorT1.setFunctionsT1(&functionsObject);
    calculatorT1.setFitter(&fitter);
    calculatorT1.setSignCalculator(&signCalculator);
    calculatorT1.setStartPointCalculator(&startPointCalculator);
    calculatorT1.setInvTimes(invTimes);
    calculatorT1.setNSamples(nSamples);
    calculatorT1.setMeanCutOff(10);

    // configure calculator itk filter
    CalculatorT1ImageFilterType::Pointer imageCalculatorItk = CalculatorT1ImageFilterType::New();
    imageCalculatorItk->SetInputMagImage(sorterMag->GetOutput());
    imageCalculatorItk->SetInputPhaImage(sorterPha->GetOutput());
    imageCalculatorItk->SetCalculator(&calculatorT1);

    // configure colornar itk filter
    Colorbar2DImageFilterType::Pointer colobarImageFilter = Colorbar2DImageFilterType::New();
    colobarImageFilter->SetInput(imageCalculatorItk->GetT1Image());

    // for comparing the images
    ComparisonImageFilter2dType::Pointer diff = ComparisonImageFilter2dType::New();
    diff->SetValidInput(readerT1Map->GetOutput());
    diff->SetTestInput(colobarImageFilter->GetOutput());

    // first update and check if any exceptions
    ASSERT_NO_THROW(imageCalculatorItk->Update());
    ASSERT_NO_THROW(colobarImageFilter->Update());

    // then compare
    diff->UpdateLargestPossibleRegion();
    ASSERT_LE(diff->GetMaximumDifference(), 2); // max allowed error is 2

    delete [] invTimes;
}

#endif //USE_PRIVATE_NR2

#endif // USE_ITK
