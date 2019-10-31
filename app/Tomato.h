/*!
 * \file Tomato.h
 * \author Konrad Werys
 * \date 2018/08/14
 */

#ifndef Tomato_Tomato_H
#define Tomato_Tomato_H

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "TomatoOptions.h"
#include "TomatoColormap.h"

#include "itkReadFileListFilter.h"
#include "itkReadDirectoryFilter.h"
#include "itkSortInvTimesImageFilter.h"
#include "itkCalculatorT1ImageFilter.h"
#include "itkColorbar2DImageFilter.h"
#include "itkNShmolliSamplesUsedTo123ImageFilter.h"

#include "itkTimeProbe.h"
// for exporting
#include "gdcmUIDGenerator.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkFileTools.h"
#include "itkAdaptImageFilter.h"

namespace Ox {

    /**
     * \class Tomato
     * includes factories and methods needed for calculation
     */
    template< typename MeasureType >
    class Tomato {

    public:
        // typedefs primitive data types
        typedef MeasureType InputPixelType;
        typedef int16_t OutputPixelType; // int does not work with some dicom viewers, be sure to use short (int16)

        // typedefs image data types
        typedef itk::Image<InputPixelType, 3> ImageType3D;
        typedef itk::Image<InputPixelType, 2> ImageType2D;
        typedef itk::Image<OutputPixelType, 2> OutputImageType;
        typedef itk::ReadFileListFilter<ImageType3D> ReadFileListFilterType;
        typedef itk::ReadDirectoryFilter<ImageType3D> ReadDirectoryFilterType;
        typedef itk::SortInvTimesImageFilter<ImageType3D, ImageType3D> SortInvTimesImageFilterType;
        typedef itk::CalculatorT1ImageFilter<ImageType3D, ImageType2D> CalculatorT1ImageFilterType;
        typedef itk::MetaDataDictionary DictionaryType;

        // member variables
        TomatoOptions<InputPixelType> *_opts;
        InputPixelType *_invTimes;
        InputPixelType *_echoTimes;
        int _nSamples;
        typename ImageType3D::Pointer _imageMag;
        typename ImageType3D::Pointer _imagePha;

        typename CalculatorT1ImageFilterType::Pointer _imageCalculatorItk;
        //SortInvTimesImageFilterType::Pointer _sorterMag;
        //SortInvTimesImageFilterType::Pointer _sorterPha;

        DictionaryType _dictionaryInput;

        /**
         * readAndSort
         * @return success/failure
         */
        int readAndSort();

        /**
         * calculate
         * @return success/failure
         */
        int calculate();

        /**
         * method so long and ugly, that I put it in a separate file
         * @return success/failure
         */
        int exportToDicom();

        /**
         * constructor
         * @param inputFileName
         */
        Tomato(std::string inputFileName);

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~Tomato();

    private:
        /**
         * We do not want the default constructor, so let's make it private.
         * I cant delete it to be compatible with c++98
         */
        Tomato(){};

        /**
         * readAndSortInputFileList
         * @return success/failure
         */
        int readAndSortInputFileList();

        /**
         * readAndSortInputDirs
         * @return success/failure
         */
        int readAndSortInputDirs();

    };

} // namespace Ox

#include "Tomato.hxx"
#include "Tomato_export.hxx"

#endif

#endif //Tomato_Tomato_H
