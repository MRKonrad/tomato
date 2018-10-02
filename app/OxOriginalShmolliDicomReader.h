/*!
 * \file OxOriginalShmolliDicomReader.h
 * \author Konrad Werys
 * \date 2018/08/24
 */

#ifndef Tomato_OxOriginalShmolliDicomReader_H
#define Tomato_OxOriginalShmolliDicomReader_H

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "itkReadFileListFilter.h"
#include "itkSortInvTimesImageFilter.h"
#include "itkExtractImageFilter.h"

#ifdef USE_VTK
#include "QuickView.h"
#endif //USE_VTK

namespace Ox {

    /**
     * \class OxOriginalShmolliDicomReader
     * includes factories and methods needed for calculation
     */
    template< typename MeasureType >
    class OriginalShmolliDicomReader {

    public:
        // typedefs primitive data types
        typedef MeasureType InputPixelType;
        typedef MeasureType OutputPixelType;

        // typedefs image data types
        typedef itk::Image<InputPixelType, 3> Image3dType;
        typedef itk::Image<OutputPixelType, 2> Image2dType;
        typedef itk::ReadFileListFilter<Image3dType> ReadFileListFilterType;
        typedef itk::SortInvTimesImageFilter<Image3dType, Image3dType> SortInvTimesImageFilterType;

        // member variables
        bool _visualise;
        InputPixelType *_invTimes;
        InputPixelType *_echoTimes;
        int _nSamples;
        typename Image3dType::Pointer _imageMag;
        typename Image3dType::Pointer _imagePha;
        typename Image2dType::Pointer _imageT1grey;
        typename Image2dType::Pointer _imageT1color;
        typename Image2dType::Pointer _imageR2;
        typename Image2dType::Pointer _imageA;
        typename Image2dType::Pointer _imageB;
        typename Image2dType::Pointer _imageT1star;
        typename Image2dType::Pointer _imageNShmolliSamplesUsed;
        typename Image2dType::Pointer _imageChiSqrt;
        typename Image2dType::Pointer _imageSnr;

        std::vector<std::string> _filesMag;
        std::vector<std::string> _filesPha;
        std::vector<std::string> _filesT1;
        std::vector<std::string> _filesFitparams;

        /**
         * readAndSort
         * @return success/failure
         */
        int readAndSort();

        /**
         * visualise
         * @return success/failure
         */
        int visualise();

        int copyFromImage(MeasureType* array, const typename Image3dType::Pointer image, const typename Image3dType::IndexType index);

        /**
         * constructor
         * @param inputFileNam
         */
        OriginalShmolliDicomReader(){
            _visualise = false;
            _invTimes = 0;
            _echoTimes = 0;
            _nSamples = 0;
            _imageMag = 0;
            _imagePha = 0;
            _imageT1grey = 0;
            _imageT1color = 0;
            _imageA = 0;
            _imageB = 0;
            _imageT1star = 0;
            _imageNShmolliSamplesUsed = 0;
            _imageChiSqrt = 0;
            _imageSnr = 0;
        };

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~OriginalShmolliDicomReader(){};
    };

} // namespace Ox

#include "OxOriginalShmolliDicomReader.hxx"

#endif

#endif //Tomato_OxOriginalShmolliDicomReader_H
