/*!
 * \file itkOxTestImage.hxx
 * \author Konrad Werys
 * \date 2018/08/13
 */

#ifndef OXShmolli2_itkOxTestImage_HXX
#define OXShmolli2_itkOxTestImage_HXX

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK


namespace Ox {

    template< typename MeasureType >
    TestItkImagesFactory< MeasureType >
    ::TestItkImagesFactory(int nRows, int nCols, std::vector <std::string> filesPaths){
        _testImage = new TestImage< MeasureType >(nRows, nCols, filesPaths);
    }

    template< typename MeasureType >
    TestItkImagesFactory< MeasureType >
    ::~TestItkImagesFactory(){
        delete _testImage;
    }

    template< typename MeasureType >
    int
    TestItkImagesFactory< MeasureType >
    ::getNSamples() const {
        return _testImage->getNSamples();
    }

    template< typename MeasureType >
    MeasureType*
    TestItkImagesFactory< MeasureType >
    ::getInvTimesPtr(){
        return _testImage->getInvTimesPtr();
    }

    template< typename MeasureType >
    std::vector<MeasureType>
    TestItkImagesFactory< MeasureType >
    ::getInvTimes() const{
        return _testImage->getInvTimes();
    }

    template< typename MeasureType >
    typename itk::Image< MeasureType, 3 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImageMag(){
        typename itk::Image< MeasureType, 3 >::Pointer image = generateImage3d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 3 >> imageIterator(image, image->GetLargestPossibleRegion());
        copyBufferToImage3d(image, _testImage->getImageMagPtr());
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 3 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImagePha(){
        typename itk::Image< MeasureType, 3 >::Pointer image = generateImage3d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 3 >> imageIterator(image, image->GetLargestPossibleRegion());
        copyBufferToImage3d(image, _testImage->getImagePhaPtr());
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 2 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImageResultsMolliA(){
        typename itk::Image< MeasureType, 2 >::Pointer image = generateImage2d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 2 >> imageIterator(image, image->GetLargestPossibleRegion());
        copyBufferToImage2d(image, _testImage->getImageResultsMolliPtr());
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 2 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImageResultsMolliB(){
        typename itk::Image< MeasureType, 2 >::Pointer image = generateImage2d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 2 >> imageIterator(image, image->GetLargestPossibleRegion());
        int nRows = _testImage->getNRows();
        int nCols = _testImage->getNCols();
        // shift pointer to point to 2rd element of the array
        MeasureType *pointer = _testImage->getImageResultsMolliPtr() + nRows*nCols;
        copyBufferToImage2d(image, pointer);
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 2 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImageResultsMolliT1star(){
        typename itk::Image< MeasureType, 2 >::Pointer image = generateImage2d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 2 >> imageIterator(image, image->GetLargestPossibleRegion());
        int nRows = _testImage->getNRows();
        int nCols = _testImage->getNCols();
        // shift pointer to point to 3rd element of the array
        MeasureType *pointer = _testImage->getImageResultsMolliPtr() + 2*nRows*nCols;
        copyBufferToImage2d(image, pointer);
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 2 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImageResultsShmolliA(){
        typename itk::Image< MeasureType, 2 >::Pointer image = generateImage2d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 2 >> imageIterator(image, image->GetLargestPossibleRegion());
        copyBufferToImage2d(image, _testImage->getImageResultsShmolliPtr());
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 2 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImageResultsShmolliB(){
        typename itk::Image< MeasureType, 2 >::Pointer image = generateImage2d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 2 >> imageIterator(image, image->GetLargestPossibleRegion());
        int nRows = _testImage->getNRows();
        int nCols = _testImage->getNCols();
        // shift pointer to point to 2rd element of the array
        MeasureType *pointer = _testImage->getImageResultsShmolliPtr() + nRows*nCols;
        copyBufferToImage2d(image, pointer);
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 2 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::gererateImageResultsShmolliT1star(){
        typename itk::Image< MeasureType, 2 >::Pointer image = generateImage2d();
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 2 >> imageIterator(image, image->GetLargestPossibleRegion());
        int nRows = _testImage->getNRows();
        int nCols = _testImage->getNCols();
        // shift pointer to point to 3rd element of the array
        MeasureType *pointer = _testImage->getImageResultsShmolliPtr() + 2*nRows*nCols;
        copyBufferToImage2d(image, pointer);
        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 2 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::generateImage2d(){

        typename itk::Image< MeasureType, 2 >::RegionType region;
        typename itk::Image< MeasureType, 2 >::IndexType start;
        start[0] = 0;
        start[1] = 0;

        typename itk::Image< MeasureType, 2 >::SizeType size;
        size[1] = _testImage->getNCols();
        size[0] = _testImage->getNRows();

        region.SetSize(size);
        region.SetIndex(start);

        typename itk::Image< MeasureType, 2 >::Pointer image = itk::Image< MeasureType, 2 >::New();
        image->SetRegions(region);
        image->Allocate();
        image->FillBuffer(0);

        return image;
    };

    template< typename MeasureType >
    typename itk::Image< MeasureType, 3 >::Pointer
    TestItkImagesFactory< MeasureType >
    ::generateImage3d(){

        typename itk::Image< MeasureType, 3 >::RegionType region;
        typename itk::Image< MeasureType, 3 >::IndexType start;
        start[0] = 0;
        start[1] = 0;
        start[2] = 0;

        typename itk::Image< MeasureType, 3 >::SizeType size;
        size[1] = _testImage->getNCols();
        size[0] = _testImage->getNRows();
        size[2] = _testImage->getNSamples();

        region.SetSize(size);
        region.SetIndex(start);

        typename itk::Image< MeasureType, 3 >::Pointer image = itk::Image< MeasureType, 3 >::New();
        image->SetRegions(region);
        image->Allocate();
        image->FillBuffer(0);

        return image;
    };

    template< typename MeasureType >
    void
    TestItkImagesFactory< MeasureType >
    ::copyBufferToImage2d(typename itk::Image< MeasureType, 2 >::Pointer image, MeasureType* buffer) {
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 2 >> imageIterator(image, image->GetLargestPossibleRegion());

        int nRows = _testImage->getNRows();
        while(!imageIterator.IsAtEnd())
        {
            int iRow = imageIterator.GetIndex()[0];
            int iCol = imageIterator.GetIndex()[1];
            imageIterator.Set(buffer[iCol * nRows + iRow]);
            ++imageIterator;
        }
    }

    template< typename MeasureType >
    void
    TestItkImagesFactory< MeasureType >
    ::copyBufferToImage3d(typename itk::Image< MeasureType, 3 >::Pointer image, MeasureType* buffer) {
        itk::ImageRegionIteratorWithIndex<itk::Image< MeasureType, 3 >> imageIterator(image, image->GetLargestPossibleRegion());

        int nRows = _testImage->getNRows();
        int nCols = _testImage->getNCols();
        while(!imageIterator.IsAtEnd())
        {
            int iRow = imageIterator.GetIndex()[0];
            int iCol = imageIterator.GetIndex()[1];
            int iSample = imageIterator.GetIndex()[2];
            imageIterator.Set(buffer[iSample * nRows * nCols+ iCol * nRows + iRow]);
            ++imageIterator;
        }
    }
} // namespace Ox


#endif // USE_ITK

#endif //OXShmolli2_itkOxTestImage_HXX
