/*!
 * \file itkPipeline_test.cpp
 * \author Konrad Werys
 * \date 2018/08/14
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "itkReadFileListFilter.h"
#include "itkSortInvTimesImageFilter.h"

#include "gtest/gtest.h"

TEST(itkReadFileListFilter, readFromMag) {

    // primitive data types
    typedef double InputPixelType;
    typedef double OutputPixelType;

    // image data types
    typedef itk::Image< InputPixelType,  3 > InputImageType3D;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::ReadFileListFilter<InputImageType3D> ReadFileListFilterType;

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

    InputPixelType temp[7] = {100, 1100, 2100, 3100, 4100, 180, 260};
    vnl_vector<InputPixelType > trueInvTimes(&temp[0], 7);
    vnl_vector<InputPixelType > readInvTimes = reader->GetInvTimes();

    for (int i = 0; i < trueInvTimes.size(); ++i) {
        ASSERT_EQ(trueInvTimes[i], readInvTimes[i]);
    }

}

TEST(itkReadFileListFilter, readFromDir) {

    // primitive data types
    typedef double InputPixelType;
    typedef double OutputPixelType;

    // image data types
    typedef itk::Image< InputPixelType,  3 > InputImageType3D;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::ReadFileListFilter<InputImageType3D> ReadFileListFilterType;

    ReadFileListFilterType::Pointer reader = ReadFileListFilterType::New();
    reader->SetDirName("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12");
    reader->Update();

    ASSERT_EQ(reader->GetInvTimes().size(), 7);
}


#endif // USE_ITK


