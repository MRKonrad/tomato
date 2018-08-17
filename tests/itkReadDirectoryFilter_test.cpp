/*!
 * \file itkPipeline_test.cpp
 * \author Konrad Werys
 * \date 2018/08/14
 */

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

#include "itkReadDirectoryFilter.h"

#include "gtest/gtest.h"

TEST(itkReadDirectoryFilter, readAndCalculateT1FromMag) {

    // primitive data types
    typedef double InputPixelType;
    typedef double OutputPixelType;

    // image data types
    typedef itk::Image< InputPixelType,  3 > InputImageType3D;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::ReadDirectoryFilter<InputImageType3D> ReadDirectoryFilterType;

    std::string dirName("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli - 1/ShMOLLI_192i_e11_12/");

    ReadDirectoryFilterType::Pointer reader = ReadDirectoryFilterType::New();
    reader->SetDirName(dirName);
    reader->Update();

    InputPixelType temp[7] = {100, 1100, 2100, 3100, 4100, 180, 260};
    vnl_vector<InputPixelType > trueInvTimes(&temp[0], 7);
    vnl_vector<InputPixelType > readInvTimes = reader->GetInvTimes();

    ASSERT_EQ(trueInvTimes, readInvTimes);
}


#endif // USE_ITK


