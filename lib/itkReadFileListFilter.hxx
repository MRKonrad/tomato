//
//  itkReadFileListFilter.h
//  TomatoLib
//
//  Created by Konrad Werys on 07/11/19.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#ifndef TomatoLIB_ITKREADFILELISTFILTER_HXX
#define TomatoLIB_ITKREADFILELISTFILTER_HXX

#include "CmakeConfigForTomato.h"
#include "gdcmTomatoReadTags.h"

#ifdef USE_ITK

#include <iostream>
#include <string>
#include "itkGDCMImageIO.h"

#include "itkStatisticsImageFilter.h"
#include "KWUtil.h"
#include "itkReadFileListFilter.h"
#include "itkDirectory.h"
#include "gdcmBase64.h"
#include "gdcmTomatoReadTags.h"

namespace itk {

    template< typename TImage >
    ReadFileListFilter<TImage>::
    ReadFileListFilter(){
        m_Verbose = false;
    }

    template< typename TImage >
    void
    ReadFileListFilter<TImage>
    ::SetFileList(const std::vector<std::string> fileList){
        m_FileList = fileList;
    }

    template<typename TImage>
    void ReadFileListFilter<TImage>::SetDirName(const std::string dirName) {

        m_DirName = dirName;
        if (m_DirName.empty()) {
            return;
        }

        typename itk::Directory::Pointer directory = itk::Directory::New();
        directory->Load(m_DirName.c_str());

        for (size_t i = 0; i < directory->GetNumberOfFiles(); i++){
            std::string fileName = directory->GetFile(i);
            if (fileName == "." || fileName == ".." || fileName == ".DS_Store"){
                continue;
            }
            m_FileList.push_back(m_DirName + KWUtil::PathSeparator() + fileName);
        }
    }

    template< typename TImage >
    template< typename TYPE >
    vnl_vector<TYPE>
    ReadFileListFilter<TImage>
    ::GetVnlVectorFromStdVector(std::vector<TYPE> stdVector){
        if (stdVector.size() == 0){
            return vnl_vector<TYPE>();
        } else {
            return vnl_vector<TYPE>(&stdVector.front(), stdVector.size());
        }
    }

    template< typename TImage >
    vnl_vector<double>
    ReadFileListFilter<TImage>
    ::GetInvTimes(){
        vnl_vector<double> invTimesVnl = GetVnlVectorFromStdVector(m_InvTimes);
        vnl_vector<double> invTimesVnl20051572 = GetVnlVectorFromStdVector(m_InvTimes20051572);
        vnl_vector<double> invTimesVnl00211189 = GetVnlVectorFromStdVector(m_InvTimes00211189);
        vnl_vector<double> invTimesVnlFromImageCommentsVnl = GetVnlVectorFromStdVector(m_InvTimesFromImageComments);
        vnl_vector<double> triggerTimesVnl = GetVnlVectorFromStdVector(m_TriggerTimes);
        if (invTimesVnl.min_value() != invTimesVnl.max_value()) {
            return invTimesVnl;
        } else if (invTimesVnl20051572.min_value() != invTimesVnl20051572.max_value()) {
            return invTimesVnl20051572;
        } else if (invTimesVnl00211189.min_value() != invTimesVnl00211189.max_value()) {
            return invTimesVnl00211189;
        } else if (invTimesVnlFromImageCommentsVnl.min_value() != invTimesVnlFromImageCommentsVnl.max_value()) {
            return invTimesVnlFromImageCommentsVnl;
        } else if (triggerTimesVnl.min_value() != triggerTimesVnl.max_value()) {
            return triggerTimesVnl;
        } else {
            return invTimesVnl;
        }
    }

    template< typename TImage >
    vnl_vector<double>
    ReadFileListFilter<TImage>
    ::GetRepTimes(){
        return GetVnlVectorFromStdVector(m_RepTimes);
    }

    template< typename TImage >
    vnl_vector<double>
    ReadFileListFilter<TImage>
    ::GetEchoTimes(){
        vnl_vector<double> echoTimesVnl = GetVnlVectorFromStdVector(m_EchoTimes);
        vnl_vector<double> echoTimes00191016Vnl = GetVnlVectorFromStdVector(m_EchoTimes00191016);
        vnl_vector<double> echoTimes00209158Vnl = GetVnlVectorFromStdVector(m_EchoTimes00209158);
        vnl_vector<double> echoTimesFromImageCommentsVnl = GetVnlVectorFromStdVector(m_EchoTimesFromImageComments);
        if (echoTimesVnl.min_value() != echoTimesVnl.max_value()) {
            return echoTimesVnl;
        } else if (echoTimesFromImageCommentsVnl.min_value() != echoTimesFromImageCommentsVnl.max_value()) {
            return echoTimesFromImageCommentsVnl;
        } else if (echoTimes00191016Vnl.min_value() != echoTimes00191016Vnl.max_value()) {
            return echoTimes00191016Vnl;
        } else if (echoTimes00209158Vnl.min_value() != echoTimes00209158Vnl.max_value()) {
            return echoTimes00209158Vnl;
        } else {
            return echoTimesVnl;
        }
    }

    template< typename TImage >
    vnl_vector<double>
    ReadFileListFilter<TImage>
    ::GetTriggerTimes(){
        return GetVnlVectorFromStdVector(m_TriggerTimes);
    }

    template< typename TImage >
    vnl_vector<double>
    ReadFileListFilter<TImage>
    ::GetAcqTimes(){
        return GetVnlVectorFromStdVector(m_AcqTimes);
    }

    template< typename TImage >
    vnl_vector<double>
    ReadFileListFilter<TImage>
    ::GetRelAcqTimes(){
        vnl_vector<double> temp = GetAcqTimes();
        double mymin = temp.min_value();
        for (unsigned int i = 0; i < temp.size(); ++i){
            temp[i] = temp[i] - mymin;
        }
        return temp;
    }

    template< class TImage >
    void
    ReadFileListFilter< TImage >
    ::GenerateData() {
        // see https://itk.org/Wiki/ITK/Examples/ImageProcessing/TileImageFilter_CreateVolume
        typename ImageType3D::Pointer outputImage;

        m_DicomIO = GDCMImageIO::New();
        m_DicomIO->LoadPrivateTagsOn();

        itk::FixedArray< PixelType, 3 > layout;
        layout[0] = 1;
        layout[1] = 1;
        layout[2] = m_FileList.size();

        // tiler
        typename TileImageType::Pointer tiler = TileImageType::New();
        tiler->SetLayout( layout );

        typename ReaderType::Pointer reader;

        // the main loop
        unsigned int inputImageNumber = 0;
        for ( unsigned int i = 0; i < m_FileList.size(); ++i ){

            //reader
            reader = ReaderType::New();
            reader->SetImageIO(m_DicomIO);
            reader->SetFileName( m_FileList[i] );
            m_DicomIO->SetFileName(m_FileList[i]);
            m_DicomIO->ReadImageInformation();
            reader->Update();

            tiler->SetInput(i, reader->GetOutput());

            try {
                tiler->Update();
                //std::cout << std::setprecision(9) << reader->GetOutput()->GetOrigin() << std::endl;
                m_MetaDataDictionaryArray.push_back(reader->GetMetaDataDictionary());
                m_InvTimes.push_back(FindInversionTime(reader));
                m_InvTimes20051572.push_back(FindInversionTime20051572(reader));
                m_InvTimes00211189.push_back(FindInversionTime00211189(reader));
                m_InvTimesFromImageComments.push_back(FindInversionTimeFromImageComments(reader));
                m_RepTimes.push_back(FindRepetitionTime(reader));
                m_EchoTimes.push_back(FindEchoTime(reader));
                m_EchoTimes00191016.push_back(FindEchoTime00191016(reader));
                m_EchoTimes00209158.push_back(FindEchoTime00209158(reader));
                m_EchoTimesFromImageComments.push_back(FindEchoTimeFromImageComments(reader));
                m_TriggerTimes.push_back(FindTriggerTime(reader));
                m_AcqTimes.push_back(FindAcqTime(reader));
                inputImageNumber++;
            } catch( itk::ExceptionObject & err ) {
                std::cerr << "Unable to read file: " << m_FileList[i] << std::endl;
                std::cerr << "ExceptionObject caught!" << std::endl << err << std::endl;
            }
        }

        if (inputImageNumber == 0) return;

        tiler->UpdateLargestPossibleRegion();
        // the final update
        outputImage = tiler->GetOutput();
        //outputImage->SetOrigin(reader->GetOutput()->GetOrigin());
        //outputImage->SetDirection(reader->GetOutput()->GetDirection());

        try {
            outputImage->Update();
        } catch( itk::ExceptionObject & err ) {
            std::cerr << "ExceptionObject caught!" << std::endl;
            std::cerr << err << std::endl;
        }

        if (GetVerbose()) {
            //
            typename ImageType3D::PixelType rescaleIntercept = m_DicomIO->GetRescaleIntercept();
            typename ImageType3D::PixelType rescaleSlope = m_DicomIO->GetRescaleSlope();

            typedef itk::StatisticsImageFilter<ImageType3D> StatisticsImageFilterType;
            typename StatisticsImageFilterType::Pointer statisticsImageFilter
                    = StatisticsImageFilterType::New();
            statisticsImageFilter->SetInput(outputImage);
            statisticsImageFilter->Update();

            std::cout << "Mean: " << statisticsImageFilter->GetMean();
            std::cout << ", Std.: " << statisticsImageFilter->GetSigma();
            std::cout << ", Min: " << statisticsImageFilter->GetMinimum();
            std::cout << ", Max: " << statisticsImageFilter->GetMaximum();

            std::cout << ", RescaleIntercept: " << rescaleIntercept << ", RescaleSlope: " << rescaleSlope << std::endl;

            //std::cout << outputImage << std::endl;

            vcl_cout << "Inversion Times: " << GetInvTimes() << vcl_endl;
            vcl_cout << "Repetition Times: " << GetRepTimes() << vcl_endl;
            vcl_cout << "Echo Times: " << GetEchoTimes() << vcl_endl;
            vcl_cout << "Trigger Times: " << GetTriggerTimes() << vcl_endl;
            vcl_cout << "Rel Acq Times: " << GetRelAcqTimes() << vcl_endl;
        }

        this->GraftOutput(outputImage);
    }

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindInversionTime(ReaderType* reader) {

        double invTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0018|0082";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);

        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr =
                    dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                invTime = KWUtil::StringToNumber<double>(tagvalue);
            }
        }
        return invTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindInversionTime20051572(ReaderType* reader) {

        double invTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "2005|1572";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);

        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr =
                    dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string decoded;
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();

                int dlen = gdcm::Base64::GetDecodeLength(tagvalue.c_str(), tagvalue.size() );
                decoded.resize(dlen);
                gdcm::Base64::Decode(&decoded[0], decoded.size(), tagvalue.c_str(), tagvalue.size());
                float * fDecoded = (float *)&decoded[0];
                invTime = (double)(*fDecoded);
            }
        }
        return invTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindInversionTime00211189(itk::ReadFileListFilter<TImage>::ReaderType *reader) {

        double invTime = 0;

        std::vector<std::pair<int, int> > tags;
        tags.push_back(std::pair<int, int>(0x5200, 0x9230));
        tags.push_back(std::pair<int, int>(0x0021, 0x11fe));
        tags.push_back(std::pair<int, int>(0x0021, 0x1189));

        std::string tagvalue;
        if (gdcmTomatoReadTags(tags, reader->GetFileName(), tagvalue) == 0) { // EXIT_SUCCESS
            invTime = *(double*)&tagvalue[0];
        }

        return invTime;
    };


    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindInversionTimeFromImageComments(ReaderType* reader) {

        int invTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0020|4000";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);

        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr = dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                std::sscanf(tagvalue.c_str(), "TIeff %i ms ", &invTime);
            }
        }
        return (double)invTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindRepetitionTime(ReaderType* reader) {

        double repTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0018|0080";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);
        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr =
                    dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                repTime = KWUtil::StringToNumber<double>(tagvalue);
            }
        }
        return repTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindEchoTime(ReaderType* reader) {

        double echoTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0018|0081";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);
        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr =
                    dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                echoTime = KWUtil::StringToNumber<double>(tagvalue);
            }
        }
        return echoTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindEchoTime00191016(ReaderType* reader) {

        double echoTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0019|1016";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);
        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr =
                    dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                echoTime = KWUtil::StringToNumber<double>(tagvalue);
            }
        }
        return echoTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindEchoTime00209158(ReaderType* reader) {

        int echoTime = 0;

        std::vector<std::pair<int, int> > tags;
        tags.push_back(std::pair<int, int>(0x5200, 0x9230));
        tags.push_back(std::pair<int, int>(0x0020, 0x9111));
        tags.push_back(std::pair<int, int>(0x0020, 0x9158));

        std::string tagvalue;
        if ( gdcmTomatoReadTags(tags, reader->GetFileName(), tagvalue) == 0) { // EXIT_SUCCESS
            std::sscanf(tagvalue.c_str(), "T2 prep. duration = %i ms", &echoTime);
        }

        return (double)echoTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindEchoTimeFromImageComments(ReaderType* reader) {

        int echoTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0020|4000";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);

        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr = dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                std::sscanf(tagvalue.c_str(), "T2 prep. duration = %i ms ", &echoTime);
            }
        }
        return (double)echoTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindTriggerTime(ReaderType* reader) {

        double triggerTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0018|1060";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);
        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr =
                    dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                triggerTime = KWUtil::StringToNumber<double>(tagvalue);
            }
        }
        return triggerTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindAcqTime(ReaderType* reader) {

        double acqTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0008|0032";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);
        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr =
                    dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                acqTime = KWUtil::dicomTime2Seconds<double>(tagvalue);
            }
        }
        return acqTime;
    }

}

#endif // USE_ITK

#endif //TomatoLIB_ITKREADFILELISTFILTER_HXX
