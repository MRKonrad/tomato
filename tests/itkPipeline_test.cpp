/*!
 * \file itkPipeline_test.cpp
 * \author Konrad Werys
 * \date 2018/08/14
 */

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

#include "OxFunctionsT1Basic.h"
#include "OxFitterAmoebaVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxStartPointCalculatorDefault3Dims.h"
#include "OxCalculatorT1Molli.h"

#include "itkReadFileListFilter.h"
#include "itkSortInvTimesImageFilter.h"
#include "itkCalculatorT1ImageFilter.h"

#ifdef USE_VTK
#include "QuickView.h"
#endif //USE_VTK

#include "gtest/gtest.h"

// TODO: implement all the tests in this file

TEST(itkPipeline, readAndCalculateT1FromMag) {

    bool doVisualise = false; //for debugging

    // primitive data types
    typedef double InputPixelType;
    typedef double OutputPixelType;

    // image data types
    typedef itk::Image< InputPixelType,  3 > InputImageType3D;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::ReadFileListFilter<InputImageType3D> ReadFileListFilterType;
    typedef itk::SortInvTimesImageFilter < InputImageType3D, InputImageType3D> SortInvTimesImageFilterType;
    typedef itk::CalculatorT1ImageFilter < InputImageType3D, OutputImageType > CalculatorT1ImageFilterType;

    std::vector<std::string> dcmMagFilePaths;
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0001.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0002.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0003.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0004.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0005.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0006.dcm");
    dcmMagFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0007.dcm");

    ReadFileListFilterType::Pointer reader = ReadFileListFilterType::New();
    reader->SetFileList(dcmMagFilePaths);
    reader->Update();

    SortInvTimesImageFilterType::Pointer sorter = SortInvTimesImageFilterType::New();
    sorter->SetInvTimesNonSorted(reader->GetInvTimes());
    sorter->SetInput(reader->GetOutput());
    sorter->Update();

    // if I do not store a copy of inversion times like this, vnl_vector does funny things with the data under
    // the pointer .data_block();
    int nSamples = sorter->GetInvTimesSorted().size();
    InputPixelType *invTimes = new InputPixelType[nSamples];
    KWUtil::copyArrayToArray(7, invTimes, sorter->GetInvTimesSorted().data_block());

    // init the necessary objects
    Ox::FunctionsT1Basic<InputPixelType> functionsObject;
    Ox::FitterAmoebaVnl<InputPixelType> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<InputPixelType> signCalculator;
    Ox::StartPointCalculatorDefault3Dims<InputPixelType> startPointCalculator;
    Ox::CalculatorT1Molli<InputPixelType> calculatorT1Molli;

    // configure fitter
    fitterAmoebaVnl.setMaxFunctionEvals(10); // to accelerate the test

    // configure calculator
    calculatorT1Molli.setFunctionsT1(&functionsObject);
    calculatorT1Molli.setFitter(&fitterAmoebaVnl);
    calculatorT1Molli.setSignCalculator(&signCalculator);
    calculatorT1Molli.setStartPointCalculator(&startPointCalculator);
    calculatorT1Molli.setInvTimes(invTimes);
    calculatorT1Molli.setNSamples(nSamples);

    // configure calculator itk filter
    CalculatorT1ImageFilterType::Pointer imageCalculatorItk = CalculatorT1ImageFilterType::New();
    imageCalculatorItk->SetInputMagImage(sorter->GetOutput());
    imageCalculatorItk->SetCalculator(&calculatorT1Molli);
    //imageCalculatorItk->SetNumberOfThreads(1);
    imageCalculatorItk->Update();

#ifdef USE_VTK
    if (doVisualise){
        // view
        QuickView viewer;
        viewer.AddImage(imageCalculatorItk->GetAImage(), true, "A");
        viewer.AddImage(imageCalculatorItk->GetBImage(), true, "B");
        viewer.AddImage(imageCalculatorItk->GetT1starImage(), true, "T1star");
        viewer.Visualize();
    }
#endif

    ASSERT_TRUE(true);

    delete [] invTimes;
}

//TEST(itkPipeline, calculateT1FromMagAndPhase) {
//
//    std::vector<std::string> dcmMagFilePaths;
//    std::vector<std::string> dcmPhaFilePaths;
//
//    ASSERT_TRUE(false);
//}
//
//TEST(itkPipeline, calculateShmolliAndCompareWithDicom) {
//
//    std::vector<std::string> dcmMagFilePaths;
//    std::vector<std::string> dcmPhaFilePaths;
//    std::vector<std::string> dcmT1MapFilePaths;
//
//    ASSERT_TRUE(false);
//}
//
//TEST(itkPipeline, calculateShmolliAndFitparamsAndCompareWithDicom) {
//
//    std::vector<std::string> dcmMagFilePaths;
//    std::vector<std::string> dcmPhaFilePaths;
//    std::vector<std::string> dcmT1MapFilePaths;
//    std::vector<std::string> dcmFitparamsFilePaths;
//
//    ASSERT_TRUE(false);
//}

#endif // USE_ITK


