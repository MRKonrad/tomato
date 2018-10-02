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
    ::visualise(){
#ifdef USE_VTK

        // visual presentation of the results
        if (_visualise) {

//            // see if pha is ok
//            typedef itk::ExtractImageFilter< Image3dType, Image2dType > ExtractFilterType;
//            ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
//
//            ExtractFilterType::InputImageIndexType extrindex;
//            extrindex.Fill(0);
//            extrindex[2] = 6;
//            ExtractFilterType::InputImageSizeType size = _imagePha->GetLargestPossibleRegion().GetSize();
//            size[2] = 0;
//            ExtractFilterType::InputImageRegionType region = _imagePha->GetLargestPossibleRegion();
//            region.SetSize(size);
//            region.SetIndex(extrindex);
//            extractFilter->SetExtractionRegion(region);
//            extractFilter->SetInput(_imagePha);
//            extractFilter->SetDirectionCollapseToSubmatrix();

            QuickView viewer;
            viewer.AddImage(_imageT1color.GetPointer(), true, "T1 color");
            viewer.AddImage(_imageT1grey.GetPointer(), true, "T1 grey");
            viewer.AddImage(_imageR2.GetPointer(), true, "R2");
            viewer.AddImage(_imageA.GetPointer(), true, "A");
            viewer.AddImage(_imageB.GetPointer(), true, "B");
            viewer.AddImage(_imageT1star.GetPointer(), true, "T1*");
            viewer.AddImage(_imageChiSqrt.GetPointer(), true, "ChiSqrt");
            viewer.AddImage(_imageSnr.GetPointer(), true, "SNR");
            viewer.AddImage(_imageNShmolliSamplesUsed.GetPointer(), true, "Number of Samples used in reconstruction");
            viewer.AddImage(_imageChiSqrt.GetPointer(), true, "ChiSqrt");
            //viewer.SetViewPortSize(500);
            viewer.Visualize();
        }

#else // USE_VTK
        printf("Visualisation not possible: the project was configured not to use VTK. Install VTK and reconfigure the project in CMake.");
        //throw itk::ExceptionObject(__FILE__, __LINE__, "Visualisation not possible: the project was configured not to use VTK. Install VTK and reconfigure the project in CMake.");
#endif // USE_VTK
        return 0; //EXIT_SUCCESS
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
