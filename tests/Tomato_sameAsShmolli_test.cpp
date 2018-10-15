/*!
 * \file Tomato_sameAsShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "gtest/gtest.h"
#include "Tomato.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2

#ifdef USE_ITK
#include "itkImageFileReader.h"
#include "itkTestingComparisonImageFilter.h"

// TODO: make sure all fitparams give the same results

TEST(Tomato, sameAsShmolli) {

    bool doVisualise = false; //for debugging

    // primitive data types
    typedef int PixelType;

    // image data types
    typedef itk::Image< PixelType, 2 > ImageType2D;

    typedef itk::ImageFileReader<ImageType2D> ReadImage2dFilterType;
    typedef itk::Testing::ComparisonImageFilter<ImageType2D, ImageType2D> ComparisonImageFilter2dType;

    std::string configFilePath = "testData/Hcmr_Phantom_1916_Shmolli_192i_e11___Shmolli_config___fileList.yaml";

    std::vector<std::string> originalFilePaths;
    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_T1MAP_14/IM-0003-0001.dcm");
//    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0001.dcm");
    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0002.dcm");
    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0003.dcm");
    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0004.dcm");
//    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0005.dcm");
//    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0006.dcm");
//    originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0007.dcm");

    Ox::Tomato<double> Tomato_object(configFilePath);
    Tomato_object.readAndSort();
    Tomato_object.calculate();
    Tomato_object.exportToDicom();

    std::string output_map_series_number = KWUtil::NumberToString(Tomato_object._opts->output_map_series_number);
    std::string output_fitparams_series_number = KWUtil::NumberToString(Tomato_object._opts->output_fitparams_series_number);

    std::vector<std::string> calculatedFilePaths;
    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_map + "/" + output_map_series_number + "_T1.dcm");
//    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_fitparams + "/" + output_fitparams_series_number + "_R2.dcm");
    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_fitparams + "/" + output_fitparams_series_number + "_A.dcm");
    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_fitparams + "/" + output_fitparams_series_number + "_B.dcm");
    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_fitparams + "/" + output_fitparams_series_number + "_T1star.dcm");
//    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_fitparams + "/" + output_fitparams_series_number + "_ShMolliRange.dcm");
//    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_fitparams + "/" + output_fitparams_series_number + "_RelSNRFinal.dcm");
//    calculatedFilePaths.push_back(Tomato_object._opts->dir_output_fitparams + "/" + output_fitparams_series_number + "_nAmebaCallFinal.dcm");

    ReadImage2dFilterType::Pointer readerOriginalT1Map = ReadImage2dFilterType::New();
    ReadImage2dFilterType::Pointer readerCalculatedT1Map = ReadImage2dFilterType::New();
    ComparisonImageFilter2dType::Pointer diff = ComparisonImageFilter2dType::New();

    for (unsigned int i = 0; i < calculatedFilePaths.size(); ++i) {
        readerOriginalT1Map->SetFileName(originalFilePaths.at(i));
        readerCalculatedT1Map->SetFileName(calculatedFilePaths.at(i));

        // for comparing the images
        diff->SetValidInput(readerOriginalT1Map->GetOutput());
        diff->SetTestInput(readerCalculatedT1Map->GetOutput());
        diff->Update();

        std::cout << diff->GetMaximumDifference() << std::endl;

        // compare
        diff->UpdateLargestPossibleRegion();
        ASSERT_LE(diff->GetMaximumDifference(), 2); // max allowed error is 2
    }

#ifdef USE_VTK
    if (doVisualise){
        // view
        QuickView viewer;
        viewer.AddImage(readerCalculatedT1Map->GetOutput(), true, "T1 calculated");
        viewer.AddImage(readerOriginalT1Map->GetOutput(), true, "T1 original");
        viewer.Visualize();
    }
#endif // USE_VTK

}

#endif // USE_PRIVATE_NR2

#endif // USE_ITK