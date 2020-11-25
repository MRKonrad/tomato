/*!
 * \file OxOriginalShmolliDicomReader.hxx
 * \author Konrad Werys
 * \date 2018/08/24
 */

#ifndef Tomato_OxOriginalShmolliDicomReader_HXX
#define Tomato_OxOriginalShmolliDicomReader_HXX

#include <itkImageFileReader.h>
#include "OxOriginalShmolliDicomReader.h"

namespace Ox {

    template< typename MeasureType >
    int
    OriginalShmolliDicomReader<MeasureType>
    ::readAndSort() {

        // Mag
        typename ReadFileListFilterType::Pointer readerMag = ReadFileListFilterType::New();
        readerMag->SetFileList(_filesMag);
        readerMag->Update();

        typename SortInvTimesImageFilterType::Pointer sorterMag = SortInvTimesImageFilterType::New();
        sorterMag->SetInvTimesNonSorted(readerMag->GetInvTimes());
        sorterMag->SetInput(readerMag->GetOutput());
        sorterMag->Update();

        // pha
        typename ReadFileListFilterType::Pointer readerPha = ReadFileListFilterType::New();
        readerPha->SetFileList(_filesPha);
        readerPha->Update();

        typename SortInvTimesImageFilterType::Pointer sorterPha = SortInvTimesImageFilterType::New();
        sorterPha->SetInvTimesNonSorted(readerPha->GetInvTimes());
        sorterPha->SetInput(readerPha->GetOutput());
        sorterPha->Update();

        if (sorterMag->GetInvTimesSorted() == sorterPha->GetInvTimesSorted()){
            vnl_vector<InputPixelType > temp = sorterMag->GetInvTimesSorted();
            _nSamples = temp.size();
            delete [] _invTimes;
            _invTimes = new InputPixelType[_nSamples];
            KWUtil::copyArrayToArray(_nSamples, _invTimes, temp.data_block());
        } else {
            throw std::runtime_error("Mag and Pha inv times are not equal");
        }

        _imageMag = sorterMag->GetOutput();
        _imagePha = sorterPha->GetOutput();

        if (_filesT1.size() > 0) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesT1.at(0));
            reader->Update();
            _imageT1color = reader->GetOutput();
        }

        if (_filesT1.size() > 1) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesT1.at(1));
            reader->Update();
            _imageT1grey = reader->GetOutput();
        }

        if (_filesFitparams.size() > 0) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesFitparams.at(0));
            reader->Update();
            _imageR2 = reader->GetOutput();
        }

        if (_filesFitparams.size() > 1) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesFitparams.at(1));
            reader->Update();
            _imageA = reader->GetOutput();
        }

        if (_filesFitparams.size() > 2) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesFitparams.at(2));
            reader->Update();
            _imageB = reader->GetOutput();
        }

        if (_filesFitparams.size() > 3) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesFitparams.at(3));
            reader->Update();
            _imageT1star = reader->GetOutput();
        }

        if (_filesFitparams.size() > 4) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesFitparams.at(4));
            reader->Update();
            _imageNShmolliSamplesUsed = reader->GetOutput();
        }

        if (_filesFitparams.size() > 5) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesFitparams.at(5));
            reader->Update();
            _imageChiSqrt = reader->GetOutput();
        }

        if (_filesFitparams.size() > 6) {
            typename itk::ImageFileReader<Image2dType>::Pointer reader = itk::ImageFileReader<Image2dType>::New();
            reader->SetFileName(_filesFitparams.at(6));
            reader->Update();
            _imageSnr = reader->GetOutput();
        }

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    int
    OriginalShmolliDicomReader<MeasureType>
    ::copyFromImage(MeasureType* array, const typename Image3dType::Pointer image, const typename Image3dType::IndexType index){

        typename Image3dType::SizeType size = image->GetLargestPossibleRegion().GetSize();
        typename Image3dType::IndexType tempIndex = index;

        for (int i = 0; i < size[2]; ++i){
            tempIndex[2] = i;
            array[i] = image->GetPixel(tempIndex);
        }

        return 0; //EXIT_SUCCESS
    }

} // namespace Ox


#endif //Tomato_OxOriginalShmolliDicomReader_H
