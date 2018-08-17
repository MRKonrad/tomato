//
//  ReadFileListFilter
//  OxShmolliLib
//
//  Created by Konrad Werys on 19/08/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#ifndef OXSHMOLLILIB_ITKREADFILELISTFILTER_H
#define OXSHMOLLILIB_ITKREADFILELISTFILTER_H

#include <iostream>
#include "itkImageSource.h"
#include "itkImageFileReader.h"
#include "itkTileImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkMetaDataObject.h"
#include "itkStatisticsImageFilter.h"
#include "KWUtil.h"

namespace itk {
    template< typename TImage>
    class ReadFileListFilter :public ImageSource< TImage > {
    public:

        /** Standard class typedefs. */
        typedef ReadFileListFilter Self;
        typedef ImageSource <TImage> Superclass;
        typedef SmartPointer <Self> Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */

        typedef TImage ImageType3D;
        typedef typename ImageType3D::PixelType PixelType;
        typedef itk::Image< PixelType, 2 > ImageType2D;
        typedef itk::ImageFileReader< ImageType3D > ReaderType;
        typedef itk::TileImageFilter< ImageType3D, ImageType3D > TileImageType;

        typedef std::vector<std::string> FileListType;
        typedef std::vector<MetaDataDictionary> MetaDataDictionaryArrayType;

        // setters
        void SetFileList(std::vector<std::string> fileList){
            m_FileList = fileList;
        }
        //itkSetMacro(MetaDataDictionaryArray, MetaDataDictionaryArrayType);
        itkSetMacro(Verbose, bool);

        template< typename TYPE >
        vnl_vector<TYPE> GetVnlVectorFromStdVector(std::vector<TYPE> stdVector){
            if (stdVector.size() == 0){
                return vnl_vector<TYPE>();
            } else {
                return vnl_vector<TYPE>(&stdVector.front(), stdVector.size());
            }
        }

        // getters
        vnl_vector<double> GetInvTimes(){
            vnl_vector<double> invTimeVnl = GetVnlVectorFromStdVector(m_InvTimes);
			      vnl_vector<double> invTimeFromImageCommentsVnl = GetVnlVectorFromStdVector(m_InvTimesFromImageComments);
			      vnl_vector<double> triggerTimesVnl = GetVnlVectorFromStdVector(m_TriggerTimes);
            //vnl_vector<double> invTimeVnl(m_InvTimes.data(),m_InvTimes.size());
            //vnl_vector<double> invTimeFromImageCommentsVnl(m_InvTimesFromImageComments.data(),m_InvTimesFromImageComments.size());
            //vnl_vector<double> triggerTimesVnl(m_TriggerTimes.data(),m_TriggerTimes.size());
            if (invTimeVnl.min_value() != invTimeVnl.max_value()) {
                return invTimeVnl;
            } else if (invTimeFromImageCommentsVnl.min_value() != invTimeFromImageCommentsVnl.max_value()) {
                return invTimeFromImageCommentsVnl;
            } else {
                return triggerTimesVnl;
            }
        }

        vnl_vector<double> GetRepTimes(){
            return GetVnlVectorFromStdVector(m_RepTimes);
            //return vnl_vector<double>(m_RepTimes.data(),m_RepTimes.size());
        }

        vnl_vector<double> GetEchoTimes(){
            return GetVnlVectorFromStdVector(m_EchoTimes);
            //return vnl_vector<double>(m_EchoTimes.data(),m_EchoTimes.size());
        }

        vnl_vector<double> GetTriggerTimes(){
            return GetVnlVectorFromStdVector(m_TriggerTimes);
            //return vnl_vector<double>(m_TriggerTimes.data(),m_TriggerTimes.size());
        }

        vnl_vector<double> GetAcqTimes(){
            return GetVnlVectorFromStdVector(m_AcqTimes);
            //return vnl_vector<double>(m_RelAcqTimes.data(),m_RelAcqTimes.size());
        }

        vnl_vector<double> GetRelAcqTimes(){
            vnl_vector<double> temp = GetAcqTimes();
            double mymin = temp.min_value();
            for (unsigned int i = 0; i < temp.size(); ++i){
                temp[i] = temp[i] - mymin;
            }
            return temp;
        }

        itkGetMacro(MetaDataDictionaryArray, MetaDataDictionaryArrayType);
        itkGetObjectMacro(DicomIO, GDCMImageIO);
        itkGetMacro(Verbose, bool);

    protected:
        ReadFileListFilter(){
            m_Verbose = false;
        };
        ~ReadFileListFilter(){};

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(ReadFileListFilter);

        std::vector<std::string> m_FileList;
        std::vector<double> m_InvTimes;
        std::vector<double> m_InvTimesFromImageComments;
        std::vector<double> m_RepTimes;
        std::vector<double> m_EchoTimes;
        std::vector<double> m_TriggerTimes;
        std::vector<double> m_AcqTimes;
        MetaDataDictionaryArrayType m_MetaDataDictionaryArray;
        GDCMImageIO::Pointer m_DicomIO;
        bool m_Verbose;

        double FindInversionTime(ReaderType* reader);
        double FindInversionTimeFromImageComments(ReaderType* reader);
        double FindRepetitionTime(ReaderType* reader);
        double FindEchoTime(ReaderType* reader);
        double FindTriggerTime(ReaderType* reader);
        double FindAcqTime(ReaderType* reader);
        double AcqTime2Seconds(std::string acqTimes);
    };

    template< class TImage >
    void
    ReadFileListFilter< TImage >
    ::GenerateData() {
        // see https://itk.org/Wiki/ITK/Examples/ImageProcessing/TileImageFilter_CreateVolume
        typename ImageType3D::Pointer outputImage;

        m_DicomIO = GDCMImageIO::New();

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
            //reader->MetaDataDictionaryArrayUpdateOn();
            tiler->SetInput(i,reader->GetOutput());

            try {
                tiler->Update();
                //std::cout << std::setprecision(9) << reader->GetOutput()->GetOrigin() << std::endl;
                m_MetaDataDictionaryArray.push_back(reader->GetMetaDataDictionary());
                m_InvTimes.push_back(FindInversionTime(reader));
                m_InvTimesFromImageComments.push_back(FindInversionTimeFromImageComments(reader));
                m_RepTimes.push_back(FindRepetitionTime(reader));
                m_EchoTimes.push_back(FindEchoTime(reader));
                m_TriggerTimes.push_back(FindTriggerTime(reader));
                m_AcqTimes.push_back(FindAcqTime(reader));
                inputImageNumber++;
            } catch( itk::ExceptionObject & err ) {
                std::cerr << "Unable to read file" << m_FileList[i] << std::endl;
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

        typedef itk::MetaDataDictionary DictionaryType;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;
        double invTime = 0;

        const  DictionaryType & dictionary = reader->GetMetaDataDictionary();
        DictionaryType::ConstIterator end = dictionary.End();

        std::string entryId = "0018|0082";
        DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);

        if ( tagItr != end ) {
            MetaDataStringType::ConstPointer entryvalueStr = dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

            if (entryvalueStr) {
                std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
                invTime = KWUtil::StringToNumber<double>(tagvalue);
                //invTime = (std::stof(tagvalue));
            }
        }
        return invTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindInversionTimeFromImageComments(ReaderType* reader) {

        typedef itk::MetaDataDictionary DictionaryType;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;
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

        typedef itk::MetaDataDictionary   DictionaryType;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;
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
                //repTime = (std::stof(tagvalue));
            }
        }
        return repTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindEchoTime(ReaderType* reader) {

        typedef itk::MetaDataDictionary   DictionaryType;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;
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
                //echoTime = (std::stof(tagvalue));
            }
        }
        return echoTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindTriggerTime(ReaderType* reader) {

        typedef itk::MetaDataDictionary   DictionaryType;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;
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
                //triggerTime = (std::stof(tagvalue));
            }
        }
        return triggerTime;
    };

    template< class TImage>
    double
    ReadFileListFilter< TImage>
    ::FindAcqTime(ReaderType* reader) {

        typedef itk::MetaDataDictionary   DictionaryType;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;
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
    };

}

#endif //OXSHMOLLILIB_ITKREADFILELISTFILTER_H
