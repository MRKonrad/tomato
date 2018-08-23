/*!
 * \file OxShmolli2.h
 * \author Konrad Werys
 * \date 2018/08/14
 */

#ifndef OXSHMOLLI2_OXSHMOLLI2_EXPORT_HXX
#define OXSHMOLLI2_OXSHMOLLI2_EXPORT_HXX

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

namespace Ox {

    int
    OxShmolli2
    ::exportToDicom(){

        if (_opts->dir_output_map.empty()){
            printf("No DICOM export, dir_output_map not given.\n");
            return EXIT_FAILURE;
        }

        // OxColorbarImageFilter
        typedef itk::Colorbar2DImageFilter< OutputImageType > OxColorbarImageFilterType;
        OxColorbarImageFilterType::Pointer OxColorbarFilter = OxColorbarImageFilterType::New();
        OxColorbarFilter->SetInput(_imageCalculatorItk->GetT1Image());

        // get the dictionary ready before export
        //typedef itk::MetaDataDictionary DictionaryType;
        //DictionaryType & _dictionaryInput = reader->GetDicomIO()->GetMetaDataDictionary();

        // T1 map with color
        DictionaryType dictionaryOutput_T1Color(_dictionaryInput);

        // UIDs
        gdcm::UIDGenerator sopuid;
        std::string sopInstanceUID_T1Color = sopuid.Generate();
        gdcm::UIDGenerator suid;
        std::string seriesUID_T1 = suid.Generate();

        // seriesNumber
        std::string seriesNumber, newSeriesNumber_T1;
        itk::ExposeMetaData<std::string>(_dictionaryInput, "0020|0011", seriesNumber);
        if (_opts->output_map_series_number == 0){
            newSeriesNumber_T1 = KWUtil::NumberToString(KWUtil::StringToNumber<int>(seriesNumber) + 10002);
            // newSeriesNumber_T1 = std::to_string( stoi(seriesNumber) + 10002 ); // C++11 needed to do this
        } else {
            newSeriesNumber_T1 = KWUtil::NumberToString(_opts->output_map_series_number);
        }

        // dicom tags dependent on the MagneticFieldStrength
        std::string MagneticFieldStrength;
        itk::ExposeMetaData<std::string>(_dictionaryInput, "0018|0087", MagneticFieldStrength);
        std::string ImageCommentsT1mapWithColormap, clutRedString, clutGreenString, clutBlueString;
        if (MagneticFieldStrength.find("1.5") != std::string::npos){
            ImageCommentsT1mapWithColormap = ImageCommentsT1mapWithColormap.append("OxShMOLLI color 980SD265ms");
            clutRedString = getEncodedSting(getShmolliColormapRed15TArray());
            clutGreenString = getEncodedSting(getShmolliColormapGreen15TArray());
            clutBlueString = getEncodedSting(getShmolliColormapBlue15TArray());
            printf("Colormap for MagneticFieldStrength 1.5T is used.\n");
        } else if (MagneticFieldStrength.find("3") != std::string::npos){
            ImageCommentsT1mapWithColormap = ImageCommentsT1mapWithColormap.append("OxShMOLLI color 1198SD175ms");
            clutRedString = getEncodedSting(getShmolliColormapRed3TArray());
            clutGreenString = getEncodedSting(getShmolliColormapGreen3TArray());
            clutBlueString = getEncodedSting(getShmolliColormapBlue3TArray());
            printf("Colormap for MagneticFieldStrength 3T is used.\n");
        } else {
            printf("MagneticFieldStrength not detected, colormap will not be used.\n");
        }

        // PaletteColorLookupTableUID
        std::string PaletteColorLookupTableUID;
        itk::ExposeMetaData<std::string>(_dictionaryInput, "0028|1199", PaletteColorLookupTableUID);
        //std::cout << "PaletteColorLookupTableUID: " << PaletteColorLookupTableUID << std::endl;

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0008|0018"), sopInstanceUID_T1Color);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0002|0003"), sopInstanceUID_T1Color);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0020|000e"), seriesUID_T1);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1199"), PaletteColorLookupTableUID); // color look up table UID

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0020|0011"), newSeriesNumber_T1); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0020|4000"), ImageCommentsT1mapWithColormap); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0020|0013"), std::string("1")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1050"), std::string("2048")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1051"), std::string("4096")); // window width

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1101"), std::string("4096\\0\\16")); // red color look up table descriptor
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1102"), std::string("4096\\0\\16")); // green color look up table descriptor
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1103"), std::string("4096\\0\\16")); // blue color look up table descriptor

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1201"), clutRedString);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1202"), clutGreenString);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1Color, std::string("0028|1203"), clutBlueString);

        // T1 map no color
        DictionaryType dictionaryOutput_T1(_dictionaryInput);

        std::string sopInstanceUID_T1 = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0008|0018"), sopInstanceUID_T1);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0002|0003"), sopInstanceUID_T1);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0020|000e"), seriesUID_T1);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0020|0011"), newSeriesNumber_T1); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0020|4000"), std::string("OxShMOLLI T1 map")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0020|0013"), std::string("2")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0028|1050"), std::string("2048")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1, std::string("0028|1051"), std::string("4096")); // window width

        // R2 map
        DictionaryType dictionaryOutput_R2(_dictionaryInput);

        std::string sopInstanceUID_R2 = sopuid.Generate();
        std::string seriesUID_R2 = suid.Generate();

        std::string newSeriesNumber_R2;
        itk::ExposeMetaData<std::string>(_dictionaryInput, "0020|0011", seriesNumber);
        if (_opts->output_fitparams_series_number == 0) {
            newSeriesNumber_R2 = KWUtil::NumberToString(KWUtil::StringToNumber<int>(seriesNumber) + 10003);
            //newSeriesNumber_R2 = std::to_string( std::stoi(seriesNumber) + 10003 ); // C++11
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
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0028|1199"), PaletteColorLookupTableUID); // color look up table UID

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0020|0011"), newSeriesNumber_R2); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_A, std::string("0020|4000"), std::string("Signal(TI)=[___AMap___]-B*exp(-TI/T1star)")); // series number
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

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0020|4000"), std::string("Signal(TI)=A-[___BMap___]*exp(-TI/T1star)")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0020|0013"), std::string("5")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|1050"), std::string("2000")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_B, std::string("0028|1051"), std::string("4000")); // window width

        // T1star map
        DictionaryType dictionaryOutput_T1star(_dictionaryInput);

        std::string sopInstanceUID_T1star = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0008|0018"), sopInstanceUID_T1star);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0002|0003"), sopInstanceUID_T1star);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0020|0011"), newSeriesNumber_R2); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0020|4000"), std::string("Signal(TI)=A-B*exp(-TI/[___T1starMap___])")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0020|0013"), std::string("6")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0028|1050"), std::string("2000")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_T1star, std::string("0028|1051"), std::string("4000")); // window width

        // _ShMOLLIT1range map
        DictionaryType dictionaryOutput_ShMolliT1Range(_dictionaryInput);

        std::string sopInstanceUID_ShMolliT1Range = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0008|0018"), sopInstanceUID_ShMolliT1Range);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0002|0003"), sopInstanceUID_ShMolliT1Range);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0020|0011"), newSeriesNumber_R2); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0020|4000"), std::string("_ShMOLLIT1range")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0020|0013"), std::string("7")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0028|1050"), std::string("1500")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_ShMolliT1Range, std::string("0028|1051"), std::string("3000")); // window width

        // _nAmebaCallFinal map
        DictionaryType dictionaryOutput_nAmebaCallFinal(_dictionaryInput);

        std::string sopInstanceUID_nAmebaCallFinal = sopuid.Generate();

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0008|0018"), sopInstanceUID_nAmebaCallFinal);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0002|0003"), sopInstanceUID_nAmebaCallFinal);
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0020|000e"), seriesUID_R2);

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0020|0011"), newSeriesNumber_R2); // series number

        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0020|4000"), std::string("_nAmebaCallFinal")); // series number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0020|0013"), std::string("8")); // instance number
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0028|0106"), std::string("0")); // smallest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0028|0107"), std::string("4096")); // largest pixel
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0028|1050"), std::string("2000")); // window center
        itk::EncapsulateMetaData<std::string>( dictionaryOutput_nAmebaCallFinal, std::string("0028|1051"), std::string("4000")); // window width

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

        // get the writer ready before export
        typedef itk::ImageFileWriter<OutputImageType> WriterType;
        WriterType::Pointer writer = WriterType::New();
        writer->SetUseInputMetaDataDictionary(false);

        // maybe this? http://www.cplusplus.com/reference/string/string/find_last_of/
        if (!_opts->dir_output_map.empty()) {

            printf("Saving to: %s ", _opts->dir_output_map.c_str());
            //std::cout << "Saving to: " << _opts->dir_output_map << std::flush;

            itk::FileTools::CreateDirectory(_opts->dir_output_map);

            // export T1 color
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_T1Color);

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_map + KWUtil::PathSeparator() + newSeriesNumber_T1 + "_T1color.dcm");
            writer->SetInput(OxColorbarFilter->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // export T1 color
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_T1);

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_map + KWUtil::PathSeparator() + newSeriesNumber_T1 + "_T1.dcm");
            writer->SetInput(OxColorbarFilter->GetOutput());
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
            typedef itk::MultiplyImageFilter< OutputImageType, OutputImageType, OutputImageType > MultiplyImageFilterType;
            MultiplyImageFilterType::Pointer multiplyFilter = MultiplyImageFilterType::New();
            multiplyFilter->SetInput( _imageCalculatorItk->GetR2Image() );
            multiplyFilter->SetConstant( 4000 );

            OxColorbarFilter->SetInput(multiplyFilter->GetOutput());
            OxColorbarFilter->SetZerosInsteadOfColorbar(true);

            // mkdir
            itk::FileTools::CreateDirectory(_opts->dir_output_fitparams);

            // export R2
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_R2);

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_R2.dcm");
            writer->SetInput(OxColorbarFilter->GetOutput());
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
            writer->SetFileName(
                    _opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_A.dcm");
            writer->SetInput(OxColorbarFilter->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // export B
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_B);

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_B.dcm");
            writer->SetInput(_imageCalculatorItk->GetBImage());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // export T1star
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_T1star);

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_T1star.dcm");
            writer->SetInput(_imageCalculatorItk->GetT1starImage());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // export ShMolliRange
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_ShMolliT1Range);

            typedef itk::NShmolliSamplesUsedTo123ImageFilter <OutputImageType> nShmolliSamplesUsedTo123FilterType;
            nShmolliSamplesUsedTo123FilterType::Pointer nShmolliSamplesUsedTo123Filter = nShmolliSamplesUsedTo123FilterType::New();
            nShmolliSamplesUsedTo123Filter->SetInput(_imageCalculatorItk->GetNShmolliSamplesUsedImage());

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_ShMolliRange.dcm");
            writer->SetInput(nShmolliSamplesUsedTo123Filter->GetOutput());
            writer->SetImageIO(gdcmImageIO);
            try {
                writer->Update();
            } catch (itk::ExceptionObject &e) {
                std::cerr << "Exception in file writer " << std::endl;
                std::cerr << e << std::endl;
            }

            // TODO: change chisquare to final call value
            // export nAmebaCalls
            gdcmImageIO->SetMetaDataDictionary(dictionaryOutput_nAmebaCallFinal);

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_nAmebaCallFinal.dcm");
            writer->SetInput(_imageCalculatorItk->GetChiSqrtImage());
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

            // export to dicom
            writer->SetFileName(
                    _opts->dir_output_fitparams + KWUtil::PathSeparator() + newSeriesNumber_R2 + "_RelSNRFinal.dcm");
            writer->SetInput(_imageCalculatorItk->GetSNRImage());
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

#endif //OXSHMOLLI2_OXSHMOLLI2_EXPORT_HXX
