//
// Created by Konrad Werys on 19.12.19.
//

#ifndef TOMATO_TOMATO_EXPORTT1_MAGSIGNRECOV_H
#define TOMATO_TOMATO_EXPORTT1_MAGSIGNRECOV_H

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "gdcmUIDGenerator.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkFileTools.h"
#include "itkExtractImageFilter.h"


namespace Ox {

    template< typename MeasureType >
    int
    Tomato<MeasureType>
    ::exportT1MagSignRecovToDicom(){

        if (_opts->dir_output_magSignRecov.empty()) {
            printf("No DICOM export, dir_output_magSignRecov not given.\n");
            return 1; // EXIT_FAILURE
        }

        typedef itk::AdaptImageFilter < ImageType2D, OutputImageType, CastPixelAccessor< InputPixelType, OutputPixelType > > ImageAdaptorType;
        typedef itk::ImageFileWriter<OutputImageType> WriterType;
        typedef itk::ExtractImageFilter<ImageType3D, ImageType2D> ExtractImageFilterType;

        // this one I want to reset
        itk::EncapsulateMetaData<std::string>( _dictionaryInput, std::string("0028|1052"), "0"); // Rescale intercept
        itk::EncapsulateMetaData<std::string>( _dictionaryInput, std::string("0028|1053"), "1"); // Rescale slope


        DictionaryType dictionaryOutput_MagSignRecov(_dictionaryInput);

        // UIDs
        gdcm::UIDGenerator sopuid;
        gdcm::UIDGenerator suid;

        std::string seriesUID_MagSignRecov = suid.Generate();

        // seriesNumber
        std::string seriesNumber;
        itk::ExposeMetaData<std::string>(_dictionaryInput, "0020|0011", seriesNumber);
        if (_opts->output_map_series_number == 0) {
            _opts->output_map_series_number = KWUtil::StringToNumber<int>(seriesNumber) + 10004;
        }
        std::string newSeriesNumber_T1 = KWUtil::NumberToString(_opts->output_map_series_number);

        // get GDCMImageIO ready before export
        typedef itk::GDCMImageIO ImageIOType;
        ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
        gdcmImageIO->KeepOriginalUIDOn();

        // to extract 2d image from 3d image
        typename ExtractImageFilterType::Pointer extractor = ExtractImageFilterType::New();
        extractor->SetInput(_imageCalculatorItk->GetMagSignRecovered());
        extractor->SetDirectionCollapseToIdentity();
        typename ImageType3D::SizeType size = _imageCalculatorItk->GetMagSignRecovered()->GetLargestPossibleRegion().GetSize();
        size[2] = 0;
        typename ImageType3D::IndexType extractorIndex;
        extractorIndex.Fill(0);
        typename ImageType3D::RegionType desiredRegion;
        desiredRegion.SetSize(size);
        desiredRegion.SetIndex(extractorIndex);
        extractor->SetExtractionRegion(desiredRegion);

        // to cast
        typename ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();

        // get the writer ready before export
        typename WriterType::Pointer writer = WriterType::New();
        writer->SetUseInputMetaDataDictionary(false);

        printf("Saving to: %s ", _opts->dir_output_magSignRecov.c_str());

        for (size_t iImage = 0; iImage < _opts->files_magnitude.size(); iImage++) {

            MeasureType invTime = this->_invTimes[iImage];

            std::string sopInstanceUID_MagSignRecov = sopuid.Generate(); // for every dicom

            itk::EncapsulateMetaData<std::string>(dictionaryOutput_MagSignRecov, std::string("0008|0018"), sopInstanceUID_MagSignRecov);
            itk::EncapsulateMetaData<std::string>(dictionaryOutput_MagSignRecov, std::string("0002|0003"), sopInstanceUID_MagSignRecov);
            itk::EncapsulateMetaData<std::string>(dictionaryOutput_MagSignRecov, std::string("0020|000e"), seriesUID_MagSignRecov);
            itk::EncapsulateMetaData<std::string>(dictionaryOutput_MagSignRecov, std::string("0020|0011"), newSeriesNumber_T1); // series number
            itk::EncapsulateMetaData<std::string>(dictionaryOutput_MagSignRecov, std::string("0020|0013"), KWUtil::NumberToString(iImage)); // instance number
            itk::EncapsulateMetaData<std::string>(dictionaryOutput_MagSignRecov, std::string("0020|4000"), "Tomato magnitude image with sign"); // image comments
            itk::EncapsulateMetaData<std::string>(dictionaryOutput_MagSignRecov, std::string("0018|0082"), KWUtil::NumberToString(invTime)); // inversion time

            itk::FileTools::CreateDirectory(_opts->dir_output_magSignRecov);

            // export T2 color
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_MagSignRecov);

            // export to dicom
            extractorIndex[2] = iImage;
            desiredRegion.SetIndex(extractorIndex);
            extractor->SetExtractionRegion(desiredRegion);
//            extractor->Update();

            adaptor->SetInput(extractor->GetOutput());
            writer->SetFileName(_opts->dir_output_magSignRecov + KWUtil::PathSeparator() + KWUtil::NumberToString(iImage) + ".dcm");
            writer->SetInput(adaptor->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }
        }

        std::cout << " Saved!" << std::endl;

        return 0; // EXIT_SUCCESS
    }

} // namespace Ox

#endif // USE_ITK

#endif //TOMATO_TOMATO_EXPORTT1_MAGSIGNRECOV_H
