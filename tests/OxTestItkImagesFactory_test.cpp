/*!
 * \file itkOxT1ImageFilter_test.cpp
 * \author Konrad Werys
 * \date 2018/08/13
 */

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

#include "gtest/gtest.h"
#include "OxTestItkImagesFactory.h"

#ifdef USE_VTK
#include "QuickView.h"
#endif //USE_VTK

TEST(TestItkImagesFactory, generateImagesWithoutErrros) {

    bool doVisualise = false; //for debugging

    typedef double TYPE;

    std::vector< std::string > filePaths;
    filePaths.push_back("testData/blood.yaml");
    filePaths.push_back("testData/myocardium.yaml");

    int nRows = 50;
    int nCols = 30;

    Ox::TestItkImagesFactory<TYPE> itkImagesFactory(nCols, nRows, filePaths);

    itk::Image <TYPE, 3>::Pointer imageMag = itkImagesFactory.gererateImageMag();
    itk::Image <TYPE, 3>::Pointer imagePha = itkImagesFactory.gererateImagePha();

    itk::Image <TYPE, 2>::Pointer imageMolliA = itkImagesFactory.gererateImageResultsMolliA();
    itk::Image <TYPE, 2>::Pointer imageMolliB = itkImagesFactory.gererateImageResultsMolliB();
    itk::Image <TYPE, 2>::Pointer imageMolliT1star = itkImagesFactory.gererateImageResultsMolliT1star();
    itk::Image <TYPE, 2>::Pointer imageShmolliA = itkImagesFactory.gererateImageResultsShmolliA();
    itk::Image <TYPE, 2>::Pointer imageShmolliB = itkImagesFactory.gererateImageResultsShmolliB();
    itk::Image <TYPE, 2>::Pointer imageShmolliT1star = itkImagesFactory.gererateImageResultsShmolliT1star();

#ifdef USE_VTK
    if (doVisualise){
        QuickView viewer;
        viewer.AddImage(imageMolliA.GetPointer(), true, "Molli A");
        viewer.AddImage(imageMolliB.GetPointer(), true, "Molli B");
        viewer.AddImage(imageMolliT1star.GetPointer(), true, "Molli T1star");
        viewer.AddImage(imageShmolliA.GetPointer(), true, "Shmolli A");
        viewer.AddImage(imageShmolliB.GetPointer(), true, "Shmolli B");
        viewer.AddImage(imageShmolliT1star.GetPointer(), true, "Shmolli T1start");
        viewer.Visualize();
    }
#endif

    EXPECT_TRUE(true);
}

#endif //USE_ITK

