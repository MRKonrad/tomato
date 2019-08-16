/*!
 * \file itkPipeline_test.cpp
 * \author Konrad Werys
 * \date 2018/08/14
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "itkReadDirectoryFilter.h"

#include "gtest/gtest.h"

TEST(itkReadDirectoryFilter, read) {

    // primitive data types
    typedef double InputPixelType;
    typedef double OutputPixelType;

    // image data types
    typedef itk::Image< InputPixelType,  3 > InputImageType3D;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::ReadDirectoryFilter<InputImageType3D> ReadDirectoryFilterType;

    std::string dirName("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12");

    ReadDirectoryFilterType::Pointer reader = ReadDirectoryFilterType::New();
    reader->SetDirName(dirName);
    reader->Update();

    InputPixelType temp[7] = {100, 1100, 2100, 3100, 4100, 180, 260};
    vnl_vector<InputPixelType > trueInvTimes(&temp[0], 7);
    vnl_vector<InputPixelType > readInvTimes = reader->GetInvTimes();

    for (int i = 0; i < trueInvTimes.size(); ++i) {
        ASSERT_EQ(trueInvTimes[i], readInvTimes[i]);
    }
}

#ifndef _WIN32

TEST(itkReadDirectoryFilter, read_verbose_test) {

    // primitive data types
    typedef double InputPixelType;
    typedef double OutputPixelType;

    // image data types
    typedef itk::Image< InputPixelType,  3 > InputImageType3D;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::ReadDirectoryFilter<InputImageType3D> ReadDirectoryFilterType;

    std::string dirName("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12");

    ReadDirectoryFilterType::Pointer reader = ReadDirectoryFilterType::New();
    reader->SetDirName(dirName);
    reader->SetVerbose(true);

    testing::internal::CaptureStdout();
    reader->Update();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.size(), 0);

}

#endif // _WIN32

#endif // USE_ITK


