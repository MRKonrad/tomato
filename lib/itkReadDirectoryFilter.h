//
//  ReadDirectoryFilter
//  TomatoLib
//
//  Created by Konrad Werys on 27/4/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#ifndef Tomato_READDIRECTORY_H
#define Tomato_READDIRECTORY_H

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include <iostream>
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSource.h"
#include "itkStatisticsImageFilter.h"

#include "KWUtil.h"

namespace itk
{
    /** \class ReadDirectoryFilter
     * \brief Scans the Dicom files in a given directory and reads the FIRST series found.
     * Puts the succeeding images in the 3rd dimension.
     * Needs dirName set.
     *
     * Based on:
     * https://itk.org/ITKExamples/src/IO/DICOM/DicomSeriesReadImageWrite3/Documentation.html
     * https://itk.org/Wiki/ITK/Examples/Developer/ImageSource
     * */

    template< typename TImage>
    class ReadDirectoryFilter :public ImageSource< TImage >
    {
    public:

        /** Standard class typedefs. */
        typedef ReadDirectoryFilter         Self;
        typedef ImageSource< TImage > Superclass;
        typedef SmartPointer< Self >  Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(ReadDirectoryFilter, ImageSource);

        typedef TImage ImageType;
        typedef typename ImageType::PixelType PixelType;
        typedef itk::ImageSeriesReader< ImageType > ReaderType;
        typedef typename ReaderType::DictionaryRawPointer DictionaryRawPointer;

        //itkGetClassMacro(Dictionary, DictionaryRawPointer);
        itkGetObjectMacro(Reader, ReaderType);

        itkGetMacro(DirName, std::string);
        itkSetMacro(DirName, std::string);

		itkGetMacro(InvTimes, vnl_vector<double>);
		itkSetMacro(InvTimes, vnl_vector<double>);

        itkSetMacro(Verbose, bool);
        itkGetMacro(Verbose, bool);

    protected:
        ReadDirectoryFilter(){
            m_Verbose = false;
        };
        ~ReadDirectoryFilter(){};

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

		/** looks for dicom Tags*/
		virtual vnl_vector<double> FindInversionTimes(ReaderType* reader);

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(ReadDirectoryFilter);

        std::string m_DirName;
		vnl_vector<double> m_InvTimes;
        // DictionaryRawPointer m_Dictionary;
        typename ReaderType::Pointer m_Reader;
        bool m_Verbose;
    };

    template< class TImage >
    void 
	ReadDirectoryFilter< TImage >
    ::GenerateData(){
        typedef itk::GDCMSeriesFileNames NamesGeneratorType;
        NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->AddSeriesRestriction("0008|0021");
        nameGenerator->SetGlobalWarningDisplay(false);
        nameGenerator->SetDirectory(m_DirName);

        try {

            //typedef std::vector< typename ImageType::Pointer >     ImagePointerContainer;
            typedef std::vector< typename std::string >    SeriesIdContainer;
            const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
            SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
            SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();


            if (seriesItr != seriesEnd) {
                if (GetVerbose()) {
                    std::cout << "The directory: ";
                    std::cout << m_DirName << std::endl;
                    std::cout << "Contains the following DICOM Series: ";
                    std::cout << std::endl;
                }
            } else {
                std::cout << "No DICOMs in: " << m_DirName << std::endl;
            }

            while (seriesItr != seriesEnd) {
                if (GetVerbose()) {
                    std::cout << seriesItr->c_str() << std::endl;
                }
                ++seriesItr;
            }

            seriesItr = seriesUID.begin();
            //while (seriesItr != seriesUID.end()) {
            std::string seriesIdentifier;

            seriesIdentifier = seriesItr->c_str();
            seriesItr++;

            if (GetVerbose()) {
                std::cout << "Reading: ";
                std::cout << seriesIdentifier << std::endl;
            }

            typedef std::vector<std::string> FileNamesContainer;
            FileNamesContainer fileNames;
            fileNames = nameGenerator->GetFileNames(seriesIdentifier);

            m_Reader = ReaderType::New();
            GDCMImageIO::Pointer dicomIO = GDCMImageIO::New();
            m_Reader->SetImageIO(dicomIO);
            m_Reader->SetFileNames(fileNames);

            typename ImageType::Pointer image;
            image = m_Reader->GetOutput();
            image->Update();

            if (GetVerbose()) {
                // rescale image according to Rescale Intercept and Rescale Slope
                typename ImageType::PixelType rescaleIntercept = dicomIO->GetRescaleIntercept();
                typename ImageType::PixelType rescaleSlope = dicomIO->GetRescaleSlope();

                typedef itk::StatisticsImageFilter<ImageType> StatisticsImageFilterType;
                typename StatisticsImageFilterType::Pointer statisticsImageFilter = StatisticsImageFilterType::New();
                statisticsImageFilter->SetInput(image);
                statisticsImageFilter->Update();

                std::cout << "Mean: " << statisticsImageFilter->GetMean();
                std::cout << ", Std.: " << statisticsImageFilter->GetSigma();
                std::cout << ", Min: " << statisticsImageFilter->GetMinimum();
                std::cout << ", Max: " << statisticsImageFilter->GetMaximum() << std::endl;

                std::cout << "rescaleIntercept: " << rescaleIntercept << ", rescaleSlope: " << rescaleSlope
                          << std::endl;

            }

			//typedef itk::ImageRegionIterator< ImageType> IteratorType;
			//IteratorType myiterator(image, image->GetLargestPossibleRegion());
			//myiterator.GoToBegin();
			//while (!myiterator.IsAtEnd()) {
			//	myiterator.Set(rescaleSlope * myiterator.Get() + rescaleIntercept);
			//	++myiterator;
			//}

            // get inversion times
            m_InvTimes = FindInversionTimes(m_Reader);
            if (GetVerbose()) {
                std::cout << "Inversion Times: " << m_InvTimes << std::endl << std::endl;
            }

            //this->m_Dictionary = (*(reader->GetMetaDataDictionaryArray()))[0];

			this->GraftOutput(image);

        } catch (itk::ExceptionObject &ex){
            std::cout << ex << std::endl;
        }
	};

	template< class TImage>
	vnl_vector<double>
	ReadDirectoryFilter< TImage>
	::FindInversionTimes(ReaderType* reader) {

		typedef itk::MetaDataDictionary   DictionaryType;
		typedef itk::MetaDataObject< std::string > MetaDataStringType;

		std::vector<double> invTimesStdVector;

		for (unsigned int i = 0; i < reader->GetMetaDataDictionaryArray()->size(); ++i) {

			const  DictionaryType & dictionary = *reader->GetMetaDataDictionaryArray()->at(i);

			std::string entryId = "0018|0082";
			DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);
			MetaDataStringType::ConstPointer entryvalueStr =
				dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer());

			if (entryvalueStr){
				std::string tagvalue = entryvalueStr->GetMetaDataObjectValue();
				invTimesStdVector.push_back(KWUtil::StringToNumber<double>(tagvalue));
			}
		}
		vnl_vector<double> invTimesVnl(&invTimesStdVector.front(), invTimesStdVector.size());
		return invTimesVnl;
	};
}// end namespace

#endif // USE_ITK

#endif //Tomato_READDIRECTORY_H
