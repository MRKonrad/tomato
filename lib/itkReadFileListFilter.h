//
//  itkReadFileListFilter.h
//  TomatoLib
//
//  Created by Konrad Werys on 19/08/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#ifndef TomatoLIB_ITKREADFILELISTFILTER_H
#define TomatoLIB_ITKREADFILELISTFILTER_H

#include "CmakeConfigForTomato.h"

#ifdef USE_ITK

#include <iostream>
#include "itkImageFileReaderKW.h"
#include "itkTileImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkMetaDataObject.h"

namespace itk {
    /**
     * Reads DICOM files
     * @tparam TImage
     */
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
        typedef itk::ImageFileReaderKW< ImageType3D > ReaderType;
        typedef itk::TileImageFilter< ImageType3D, ImageType3D > TileImageType;

        typedef std::vector<std::string> FileListType;
        typedef std::vector<MetaDataDictionary> MetaDataDictionaryArrayType;

        typedef itk::MetaDataDictionary   DictionaryType;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;

        // setters
        void SetFileList(std::vector<std::string> fileList);
        void SetDirName(std::string dirName);
        itkSetMacro(Verbose, bool);

        template< typename TYPE >
        vnl_vector<TYPE> GetVnlVectorFromStdVector(std::vector<TYPE> stdVector);

        // getters
        vnl_vector<double> GetInvTimes();
        vnl_vector<double> GetRepTimes();
        vnl_vector<double> GetEchoTimes();
        vnl_vector<double> GetTriggerTimes();
        vnl_vector<double> GetAcqTimes();
        vnl_vector<double> GetRelAcqTimes();

        itkGetMacro(MetaDataDictionaryArray, MetaDataDictionaryArrayType);
        itkGetObjectMacro(DicomIO, GDCMImageIO);
        itkGetMacro(Verbose, bool);

    protected:

        ReadFileListFilter();

        virtual ~ReadFileListFilter(){};

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(ReadFileListFilter);

        std::vector<std::string> m_FileList;
        std::string m_DirName;
        std::vector<double> m_InvTimes;
        std::vector<double> m_InvTimes20051572;
        std::vector<double> m_InvTimesFromImageComments;
        std::vector<double> m_RepTimes;
        std::vector<double> m_EchoTimes;
        std::vector<double> m_EchoTimes00191016;
        std::vector<double> m_EchoTimes00209158;
        std::vector<double> m_TriggerTimes;
        std::vector<double> m_AcqTimes;
        MetaDataDictionaryArrayType m_MetaDataDictionaryArray;
        GDCMImageIO::Pointer m_DicomIO;
        bool m_Verbose;

        double FindInversionTime(ReaderType* reader);
        double FindInversionTime20051572(ReaderType* reader);
        double FindInversionTimeFromImageComments(ReaderType* reader);
        double FindRepetitionTime(ReaderType* reader);
        double FindEchoTime(ReaderType* reader);
        double FindEchoTime00191016(ReaderType* reader);
        double FindEchoTime00209158(ReaderType* reader);
        double FindTriggerTime(ReaderType* reader);
        double FindAcqTime(ReaderType* reader);
    };
}

#ifndef TOMATOLIB_COMPILED
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkReadFileListFilter.hxx"
#endif //ITK_MANUAL_INSTANTIATION
#endif //TOMATOLIB_COMPILED

#endif // USE_ITK


#endif //TomatoLIB_ITKREADFILELISTFILTER_H
