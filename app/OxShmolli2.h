/*!
 * \file OxShmolli2.h
 * \author Konrad Werys
 * \date 2018/08/14
 */

#ifndef OXSHMOLLI2_OXSHMOLLI2_H
#define OXSHMOLLI2_OXSHMOLLI2_H

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

#include "OxShmolli2Options.h"
#include "OxShmolli2Colormap.h"

#include "itkReadFileListFilter.h"
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

#ifdef USE_VTK
#include "QuickView.h"
#endif //USE_VTK

namespace Ox {

    /**
     * \class OxShmolli2
     * includes factories and methods needed for calculation
     */
    template< typename MeasureType >
    class OxShmolli2 {

    public:
        // typedefs primitive data types
        typedef MeasureType InputPixelType;
        typedef MeasureType OutputPixelType;

        // typedefs image data types
        typedef itk::Image<InputPixelType, 3> InputImageType3D;
        typedef itk::Image<OutputPixelType, 2> OutputImageType;
        typedef itk::ReadFileListFilter<InputImageType3D> ReadFileListFilterType;
        typedef itk::SortInvTimesImageFilter<InputImageType3D, InputImageType3D> SortInvTimesImageFilterType;
        typedef itk::CalculatorT1ImageFilter<InputImageType3D, OutputImageType> CalculatorT1ImageFilterType;
        typedef itk::MetaDataDictionary DictionaryType;

        // member variables
        OxShmolli2Options<InputPixelType> *_opts;
        InputPixelType *_invTimes;
        InputPixelType *_echoTimes;
        int _nSamples;
        typename InputImageType3D::Pointer _imageMag;
        typename InputImageType3D::Pointer _imagePha;

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
         * visualise
         * @return success/failure
         */
        int visualise();

        /**
         * method so long and ugly, that I put it in a separate file
         * @return success/failure
         */
        int exportToDicom();

        /**
         * constructor
         * @param inputFileName
         */
        OxShmolli2(std::string inputFileName);

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~OxShmolli2();

    private:
        /**
         * We do not want the default constructor, so let's make it private.
         * I cant delete it to be compatible with c++98
         */
        OxShmolli2(){};

    };

} // namespace Ox

#include "OxShmolli2.hxx"
#include "OxShmolli2_export.hxx"

#endif

#endif //OXSHMOLLI2_OXSHMOLLI2_H
