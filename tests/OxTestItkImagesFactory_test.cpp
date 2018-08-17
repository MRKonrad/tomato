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
#include "itkExtractImageFilter.h"
#endif //USE_VTK

TEST(OxTestItkImagesFactory, generateImagesWithoutErrros) {

    bool doVisualise = false; //for debugging

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

#ifdef USE_VTK
    if (doVisualise){

        // see if pha is ok
        using ExtractFilterType = itk::ExtractImageFilter< Image3dType, Image2dType >;
        ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();

        ExtractFilterType::InputImageIndexType extrindex;
        extrindex.Fill(0);
        extrindex[2] = 6;
        ExtractFilterType::InputImageSizeType size = imagePha->GetLargestPossibleRegion().GetSize();
        size[2] = 0;
        ExtractFilterType::InputImageRegionType region = imagePha->GetLargestPossibleRegion();
        region.SetSize(size);
        region.SetIndex(extrindex);
        extractFilter->SetExtractionRegion(region);
        extractFilter->SetInput(imagePha);
        extractFilter->SetDirectionCollapseToSubmatrix();

        // add a zero pixel to check indexing
        itk::Image <TYPE, 2>::IndexType index;
        index[0] = 1;
        index[1] = 2;
        imageMolliA->SetPixel(index, 0);

        // view
        QuickView viewer;
        viewer.AddImage(extractFilter->GetOutput(), true, "imagePha 0");
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

