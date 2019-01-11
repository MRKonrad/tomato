/*!
 * \file Tomato_sameAsShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2

#ifdef USE_ITK

#include "gtest/gtest.h"
#include "Tomato.h"
#include "itkImageFileReader.h"
#include "itkTestingComparisonImageFilter.h"

// TODO: make sure all SNR give the same results
// TODO: make sure all nAmoebaFinalCall give the same results

/**
 * \class ActeptanceTests_TomatoTest
 * \brief based on https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#sharing-resources-between-tests-in-the-same-test-case
 */
class ActeptanceTests_TomatoFixtureTest : public ::testing::Test {

    // primitive data types
    typedef int16_t PixelType;

    // image data types
    typedef itk::Image< PixelType, 2 > ImageType2D;

    typedef itk::ImageFileReader<ImageType2D> ReadImage2dFilterType;
    typedef itk::Testing::ComparisonImageFilter<ImageType2D, ImageType2D> ComparisonImageFilter2dType;

protected:
    // Per-test-case set-up.
    // Called before the first test in this test case.
    // Can be omitted if not needed.
    static void SetUpTestCase() {

        std::string configFilePath = "testData/Hcmr_Phantom_1916_Shmolli_192i_e11___Shmolli_config___fileList.yaml";

        _tomato_object = new Ox::Tomato<double>(configFilePath);

        _tomato_object->readAndSort();
        _tomato_object->calculate();
        _tomato_object->exportToDicom();

        std::string output_map_series_number = KWUtil::NumberToString(_tomato_object->_opts->output_map_series_number);
        std::string output_fitparams_series_number = KWUtil::NumberToString(_tomato_object->_opts->output_fitparams_series_number);

        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_map + KWUtil::PathSeparator() + output_map_series_number + "_T1.dcm");
        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_fitparams + KWUtil::PathSeparator() + output_fitparams_series_number + "_R2.dcm");
        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_fitparams + KWUtil::PathSeparator() + output_fitparams_series_number + "_A.dcm");
        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_fitparams + KWUtil::PathSeparator() + output_fitparams_series_number + "_B.dcm");
        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_fitparams + KWUtil::PathSeparator() + output_fitparams_series_number + "_T1star.dcm");
        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_fitparams + KWUtil::PathSeparator() + output_fitparams_series_number + "_ShMolliRange.dcm");
        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_fitparams + KWUtil::PathSeparator() + output_fitparams_series_number + "_RelSNRFinal.dcm");
        _calculatedFilePaths.push_back(_tomato_object->_opts->dir_output_fitparams + KWUtil::PathSeparator() + output_fitparams_series_number + "_nAmebaCallFinal.dcm");

        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_T1MAP_14/IM-0003-0001.dcm");
        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0001.dcm");
        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0002.dcm");
        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0003.dcm");
        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0004.dcm");
        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0005.dcm");
        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0006.dcm");
        _originalFilePaths.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0007.dcm");

    }

    // Per-test-case tear-down.
    // Called after the last test in this test case.
    // Can be omitted if not needed.
    static void TearDownTestCase() {
        delete _tomato_object;
        _tomato_object = NULL;
    }

    // You can define per-test set-up logic as usual.
    virtual void SetUp() {
        readerOriginalT1Map = ReadImage2dFilterType::New();
        readerCalculatedT1Map = ReadImage2dFilterType::New();
        diff = ComparisonImageFilter2dType::New();
    }

    // You can define per-test tear-down logic as usual.
    virtual void TearDown() {  }

public:
    // Some expensive resource shared by all tests.
    static Ox::Tomato<double>* _tomato_object;
    static std::vector<std::string> _originalFilePaths;
    static std::vector<std::string> _calculatedFilePaths;

    ReadImage2dFilterType::Pointer readerOriginalT1Map;
    ReadImage2dFilterType::Pointer readerCalculatedT1Map;
    ComparisonImageFilter2dType::Pointer diff;
};

Ox::Tomato<double>* ActeptanceTests_TomatoFixtureTest::_tomato_object = NULL;
std::vector<std::string> ActeptanceTests_TomatoFixtureTest::_originalFilePaths;
std::vector<std::string> ActeptanceTests_TomatoFixtureTest::_calculatedFilePaths;



TEST_F(ActeptanceTests_TomatoFixtureTest, sameAsShmolli_T1) {

    bool doVisualise = false; //for debugging
    int i = 0; // T1 map

    readerOriginalT1Map->SetFileName(_originalFilePaths.at(i));
    readerCalculatedT1Map->SetFileName(_calculatedFilePaths.at(i));

    // for comparing the images
    diff->SetValidInput(readerOriginalT1Map->GetOutput());
    diff->SetTestInput(readerCalculatedT1Map->GetOutput());
    diff->Update();

    // compare
    diff->UpdateLargestPossibleRegion();
    ASSERT_LE(diff->GetMaximumDifference(), 2); // max allowed error is 2

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

TEST_F(ActeptanceTests_TomatoFixtureTest, sameAsShmolli_R2) {

    bool doVisualise = false; //for debugging
    int i = 1; // R2 map

    readerOriginalT1Map->SetFileName(_originalFilePaths.at(i));
    readerCalculatedT1Map->SetFileName(_calculatedFilePaths.at(i));

    // for comparing the images
    diff->SetValidInput(readerOriginalT1Map->GetOutput());
    diff->SetTestInput(readerCalculatedT1Map->GetOutput());
    diff->Update();

    // compare
    diff->UpdateLargestPossibleRegion();
    ASSERT_LE(diff->GetNumberOfPixelsWithDifferences(), 300); // max allowed error is 2

#ifdef USE_VTK
    if (doVisualise){
        // view
        QuickView viewer;
        viewer.AddImage(readerCalculatedT1Map->GetOutput(), true, "Calculated");
        viewer.AddImage(readerOriginalT1Map->GetOutput(), true, "Original");
        viewer.Visualize();
    }
#endif // USE_VTK

}

TEST_F(ActeptanceTests_TomatoFixtureTest, sameAsShmolli_A) {

    bool doVisualise = false; //for debugging
    int i = 2; // A map

    readerOriginalT1Map->SetFileName(_originalFilePaths.at(i));
    readerCalculatedT1Map->SetFileName(_calculatedFilePaths.at(i));

    // for comparing the images
    diff->SetValidInput(readerOriginalT1Map->GetOutput());
    diff->SetTestInput(readerCalculatedT1Map->GetOutput());
    diff->Update();

    // compare
    diff->UpdateLargestPossibleRegion();
    ASSERT_LE(diff->GetMaximumDifference(), 2); // max allowed error is 2

#ifdef USE_VTK
    if (doVisualise){
        // view
        QuickView viewer;
        viewer.AddImage(readerCalculatedT1Map->GetOutput(), true, "Calculated");
        viewer.AddImage(readerOriginalT1Map->GetOutput(), true, "Original");
        viewer.Visualize();
    }
#endif // USE_VTK

}

TEST_F(ActeptanceTests_TomatoFixtureTest, sameAsShmolli_B) {

    bool doVisualise = false; //for debugging
    int i = 3; // B map

    readerOriginalT1Map->SetFileName(_originalFilePaths.at(i));
    readerCalculatedT1Map->SetFileName(_calculatedFilePaths.at(i));

    // for comparing the images
    diff->SetValidInput(readerOriginalT1Map->GetOutput());
    diff->SetTestInput(readerCalculatedT1Map->GetOutput());
    diff->Update();

    // compare
    diff->UpdateLargestPossibleRegion();
    ASSERT_LE(diff->GetMaximumDifference(), 2); // max allowed error is 2

#ifdef USE_VTK
    if (doVisualise){
        // view
        QuickView viewer;
        viewer.AddImage(readerCalculatedT1Map->GetOutput(), true, "Calculated");
        viewer.AddImage(readerOriginalT1Map->GetOutput(), true, "Original");
        viewer.Visualize();
    }
#endif // USE_VTK

}

TEST_F(ActeptanceTests_TomatoFixtureTest, sameAsShmolli_T1star) {

    bool doVisualise = false; //for debugging
    int i = 4; // T1star map

    readerOriginalT1Map->SetFileName(_originalFilePaths.at(i));
    readerCalculatedT1Map->SetFileName(_calculatedFilePaths.at(i));

    // for comparing the images
    diff->SetValidInput(readerOriginalT1Map->GetOutput());
    diff->SetTestInput(readerCalculatedT1Map->GetOutput());
    diff->Update();

    // compare
    diff->UpdateLargestPossibleRegion();
    ASSERT_LE(diff->GetMaximumDifference(), 2); // max allowed error is 2

#ifdef USE_VTK
    if (doVisualise){
        // view
        QuickView viewer;
        viewer.AddImage(readerCalculatedT1Map->GetOutput(), true, "Calculated");
        viewer.AddImage(readerOriginalT1Map->GetOutput(), true, "Original");
        viewer.Visualize();
    }
#endif // USE_VTK

}

TEST_F(ActeptanceTests_TomatoFixtureTest, sameAsShmolli_ShmolliRange) {

    bool doVisualise = false; //for debugging
    int i = 5; // ShmolliRange map

    readerOriginalT1Map->SetFileName(_originalFilePaths.at(i));
    readerCalculatedT1Map->SetFileName(_calculatedFilePaths.at(i));

    // for comparing the images
    diff->SetValidInput(readerOriginalT1Map->GetOutput());
    diff->SetTestInput(readerCalculatedT1Map->GetOutput());
    diff->Update();

    // compare
    diff->UpdateLargestPossibleRegion();
    ASSERT_LE(diff->GetMaximumDifference(), 2); // max allowed error is 2

#ifdef USE_VTK
    if (doVisualise){
        // view
        QuickView viewer;
        viewer.AddImage(readerCalculatedT1Map->GetOutput(), true, "Calculated");
        viewer.AddImage(readerOriginalT1Map->GetOutput(), true, "Original");
        viewer.Visualize();
    }
#endif // USE_VTK

}

#endif // USE_PRIVATE_NR2

#endif // USE_ITK