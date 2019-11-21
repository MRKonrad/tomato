/*!
 * \file Tomato_exportT2.hxx
 * \author Konrad Werys
 * \date 2019/11/19
 */

#ifndef TOMATO_TOMATO_EXPORTT2_HXX
#define TOMATO_TOMATO_EXPORTT2_HXX

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "gdcmUIDGenerator.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkFileTools.h"
#include "itkAdaptImageFilter.h"
#include "itkThresholdImageFilter.h"

namespace Ox {

    template< typename MeasureType >
    int
    Tomato<MeasureType>
    ::exportT2ToDicom(){

        if (_opts->dir_output_map.empty()){
            printf("No DICOM export, dir_output_map not given.\n");
            return EXIT_FAILURE;
        }

        // some typedefs
        typedef itk::Colorbar2DImageFilter< ImageType2D > OxColorbarImageFilterType;
        typedef itk::AdaptImageFilter < ImageType2D, OutputImageType, CastPixelAccessor< InputPixelType, OutputPixelType > > ImageAdaptorType;
        typedef itk::ImageFileWriter<OutputImageType> WriterType;
        typedef itk::MultiplyImageFilter< ImageType2D, ImageType2D, ImageType2D > MultiplyImageFilterType;
        typedef itk::ThresholdImageFilter<ImageType2D> ThresholdImageFilterType;


        // OxColorbarImageFilter
        typename OxColorbarImageFilterType::Pointer OxColorbarFilter = OxColorbarImageFilterType::New();
        OxColorbarFilter->SetInput(_imageCalculatorItk->GetT2Image());
        OxColorbarFilter->SetAddColorbar(_opts->use_colorbar);

        // T2 map with color
        DictionaryType dictionaryOutput_T2(_dictionaryInput);

        // UIDs
        gdcm::UIDGenerator sopuid;
        std::string sopInstanceUID_T2 = sopuid.Generate();
        gdcm::UIDGenerator suid;
        std::string seriesUID_T2 = suid.Generate();

        // seriesNumber
        std::string seriesNumber, newSeriesNumber_T2;
        itk::ExposeMetaData<std::string>(_dictionaryInput, "0020|0011", seriesNumber);
        if (_opts->output_map_series_number == 0){
            newSeriesNumber_T2 = KWUtil::NumberToString(KWUtil::StringToNumber<int>(seriesNumber) + 10002);
        } else {
            newSeriesNumber_T2 = KWUtil::NumberToString(_opts->output_map_series_number);
        }

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0008|0018"), sopInstanceUID_T2);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0002|0003"), sopInstanceUID_T2);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0020|000e"), seriesUID_T2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0020|0011"), newSeriesNumber_T2); // series number
//        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0028|1052"), "0"); // rescale intercept
//        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0028|1053"), "10"); // rescale slope. Matlab is interpreting it the wrong way

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0020|4000"), "T2*10 map"); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2, std::string("0020|0013"), std::string("1")); // instance number

        // R2 map
        DictionaryType dictionaryOutput_R2(_dictionaryInput);

        std::string sopInstanceUID_R2 = sopuid.Generate();
        std::string seriesUID_R2 = suid.Generate();

        std::string newSeriesNumber_R2;
        itk::ExposeMetaData<std::string>(_dictionaryInput, "0020|0011", seriesNumber);
        if (_opts->output_fitparams_series_number == 0) {
            newSeriesNumber_R2 = KWUtil::NumberToString(KWUtil::StringToNumber<int>(seriesNumber) + 10003);
        } else {
            newSeriesNumber_R2 = KWUtil::NumberToString(_opts->output_fitparams_series_number);
        }

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0008|0018"), sopInstanceUID_R2);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0002|0003"), sopInstanceUID_R2);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0020|0011"), newSeriesNumber_R2); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0020|4000"), std::string("Rsquare*4000 Map")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0020|0013"), std::string("3")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0028|1050"), std::string("3900")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_R2, std::string("0028|1051"), std::string("200")); // window width

        // A map
        DictionaryType dictionaryOutput_A(_dictionaryInput);

        std::string sopInstanceUID_A = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0008|0018"), sopInstanceUID_A);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0002|0003"), sopInstanceUID_A);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0020|0011"), newSeriesNumber_R2); // series number

    //        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0020|4000"), std::string("Signal(TI)=[___AMap___]-B*exp(-TI/T2star)"));
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0020|4000"), std::string("A"));
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0020|0013"), std::string("4")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0028|1050"), std::string("2000")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0028|1051"), std::string("4000")); // window width

        // B map
        DictionaryType dictionaryOutput_B(_dictionaryInput);

        std::string sopInstanceUID_B = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0008|0018"), sopInstanceUID_B);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0002|0003"), sopInstanceUID_B);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0020|0011"), newSeriesNumber_R2); // series number

    //        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0020|4000"), std::string("Signal(TI)=A-[___BMap___]*exp(-TI/T2star)"));
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0020|4000"), std::string("B"));
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0020|0013"), std::string("5")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|1050"), std::string("2000")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|1051"), std::string("4000")); // window width

        // T2star map
        DictionaryType dictionaryOutput_T2star(_dictionaryInput);

        std::string sopInstanceUID_T2star = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0008|0018"), sopInstanceUID_T2star);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0002|0003"), sopInstanceUID_T2star);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0020|0011"), newSeriesNumber_R2); // series number

    //        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0020|4000"), std::string("Signal(TI)=A-B*exp(-TI/[___T2starMap___])")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0020|0013"), std::string("6")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0028|1050"), std::string("2000")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T2star, std::string("0028|1051"), std::string("4000")); // window width

        // _RelSNRFinal map
        DictionaryType dictionaryOutput_RelSNRFinal(_dictionaryInput);

        std::string sopInstanceUID_RelSNRFinal = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0008|0018"), sopInstanceUID_RelSNRFinal);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0002|0003"), sopInstanceUID_RelSNRFinal);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0020|0011"), newSeriesNumber_R2); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0020|4000"), std::string("_RelSNRFinal")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0020|0013"), std::string("9")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0028|1050"), std::string("2000")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_RelSNRFinal, std::string("0028|1051"), std::string("4000")); // window width

        // get GDCMImageIO ready before export
        typedef itk::GDCMImageIO ImageIOType;
        ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
        gdcmImageIO->KeepOriginalUIDOn();

        // to cast
        typename ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();

        // get the writer ready before export
        typename WriterType::Pointer writer = WriterType::New();
        writer->SetUseInputMetaDataDictionary(false);

        // maybe this? http://www.cplusplus.com/reference/string/string/find_last_of/
        if (!_opts->dir_output_map.empty()) {

            printf("Saving to: %s ", _opts->dir_output_map.c_str());
            //std::cout << "Saving to: " << _opts->dir_output_map << std::flush;

            itk::FileTools::CreateDirectory(_opts->dir_output_map);

            // scaling T2 * 10
            typename MultiplyImageFilterType::Pointer multiplyT2Filter = MultiplyImageFilterType::New();
            multiplyT2Filter->SetInput(OxColorbarFilter->GetOutput() );
            multiplyT2Filter->SetConstant( 10. );

            typename ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();
            thresholdFilter->SetInput(multiplyT2Filter->GetOutput());
            // thresholdFilter->SetUpper(pow(2, 16) - 1);
            thresholdFilter->SetUpper(powl(2, 12) - 1);
            thresholdFilter->SetOutsideValue(thresholdFilter->GetUpper());

            // export T2 color
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_T2);

            // export to dicom
            adaptor->SetInput(thresholdFilter->GetOutput());
            writer->SetFileName(_opts->dir_output_map + KWUtil::PathSeparator() + newSeriesNumber_T2 + "_T2.dcm");
            writer->SetInput(adaptor->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            std::cout << " Saved!" << std::endl;
        }

        if (!_opts->dir_output_fitparams.empty()) {

            printf("Saving to: %s ", _opts->dir_output_fitparams.c_str());
            //std::cout << "Saving to: " << _opts->dir_output_fitparams << std::flush;

            // scaling R2 * 4000
            typename MultiplyImageFilterType::Pointer multiplyR2Filter = MultiplyImageFilterType::New();
            multiplyR2Filter->SetInput(_imageCalculatorItk->GetR2Image() );
            multiplyR2Filter->SetConstant(4000 );

            OxColorbarFilter->SetInput(multiplyR2Filter->GetOutput());
            OxColorbarFilter->SetZerosInsteadOfColorbar(true);

            // mkdir
            itk::FileTools::CreateDirectory(_opts->dir_output_fitparams);

            // export R2
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_R2);

            // export to dicom
            adaptor->SetInput(OxColorbarFilter->GetOutput());
            writer->SetFileName(_opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_R2.dcm");
            writer->SetInput(adaptor->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // export A
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_A);

            OxColorbarFilter->SetInput(_imageCalculatorItk->GetAImage());
            OxColorbarFilter->SetZerosInsteadOfColorbar(true);

            // export to dicom
            adaptor->SetInput(OxColorbarFilter->GetOutput());
            writer->SetFileName(_opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_A.dcm");
            writer->SetInput(adaptor->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // export B
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_B);

            OxColorbarFilter->SetInput(_imageCalculatorItk->GetBImage());
            OxColorbarFilter->SetZerosInsteadOfColorbar(true);

            // export to dicom
            adaptor->SetInput(OxColorbarFilter->GetOutput());
            writer->SetFileName(_opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_B.dcm");
            writer->SetInput(adaptor->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // export T2star
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_T2star);

            OxColorbarFilter->SetInput(_imageCalculatorItk->GetT1starImage());
            OxColorbarFilter->SetZerosInsteadOfColorbar(true);

            // export to dicom
            adaptor->SetInput(OxColorbarFilter->GetOutput());
            writer->SetFileName(_opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_T2star.dcm");
            writer->SetInput(adaptor->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // TODO: check, values seem a little low
            // export RelSNRFinal
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_RelSNRFinal);

            OxColorbarFilter->SetInput(_imageCalculatorItk->GetSNRImage());
            OxColorbarFilter->SetZerosInsteadOfColorbar(true);

            // export to dicom
            adaptor->SetInput(OxColorbarFilter->GetOutput());
            writer->SetFileName(_opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_RelSNRFinal.dcm");
            writer->SetInput(adaptor->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }
            std::cout << " Saved!" << std::endl;
        }

        return 0; // EXIT_SUCCESS
    }

} // namespace Ox

#endif

#endif //TOMATO_TOMATO_EXPORTT2_HXX
