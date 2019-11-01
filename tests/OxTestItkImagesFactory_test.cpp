/*!
 * \file itkOxT1ImageFilter_test.cpp
 * \author Konrad Werys
 * \date 2018/08/13
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "gtest/gtest.h"
#include "OxTestItkImagesFactory.h"

TEST(OxTestItkImagesFactory, generateImagesWithoutErrros) {

    typedef double TYPE;
    typedef itk::Image <TYPE, 2> Image2dType;
    typedef itk::Image <TYPE, 3> Image3dType;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 30; //y
    int nCols = 20; //x

    Ox::TestItkImagesFactory<TYPE> itkImagesFactory(nRows, nCols, filePaths);

    Image3dType::Pointer imageMag = itkImagesFactory.generateImageMag();
    Image3dType::Pointer imagePha = itkImagesFactory.generateImagePha();

    Image2dType::Pointer imageMolliA = itkImagesFactory.generateImageResultsMolliA();
    Image2dType::Pointer imageMolliB = itkImagesFactory.generateImageResultsMolliB();
    Image2dType::Pointer imageMolliT1star = itkImagesFactory.generateImageResultsMolliT1star();
    Image2dType::Pointer imageShmolliA = itkImagesFactory.generateImageResultsShmolliA();
    Image2dType::Pointer imageShmolliB = itkImagesFactory.generateImageResultsShmolliB();
    Image2dType::Pointer imageShmolliT1star = itkImagesFactory.generateImageResultsShmolliT1star();

    EXPECT_EQ(imageMag.IsNull(), false);
    EXPECT_EQ(imagePha.IsNull(), false);
    EXPECT_EQ(imageMolliA.IsNull(), false);
    EXPECT_EQ(imageMolliB.IsNull(), false);
    EXPECT_EQ(imageMolliT1star.IsNull(), false);
    EXPECT_EQ(imageShmolliA.IsNull(), false);
    EXPECT_EQ(imageShmolliB.IsNull(), false);
    EXPECT_EQ(imageShmolliT1star.IsNull(), false);

}

#endif //USE_ITK

