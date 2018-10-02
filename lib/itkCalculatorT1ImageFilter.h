/*!
 * \file itkCalculatorT1ImageFilter.h
 * \author Konrad Werys
 * \date 2018/08/13
 */

#ifndef itkCalculatorT1ImageFilter_h
#define itkCalculatorT1ImageFilter_h

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include "itkLogger.h"
#include "KWUtil.h"
#include "OxFunctionsT1.h"
#include "OxFitter.h"
#include "OxCalculatorT1.h"

#include "itkDataObject.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionSplitterDirection.h"
#include "itkImageRegionIterator.h"
#include "itkImageLinearIteratorWithIndex.h"


namespace itk {

    /**
     * \class CalculatorT1ImageFilter
     * @tparam TImageIn
     * @tparam TImageOut
     */
    template<typename TImageIn, typename TImageOut>
    class CalculatorT1ImageFilter : public ImageToImageFilter<TImageIn, TImageOut> {
    public:
        /** Standard class typedefs. */
        typedef CalculatorT1ImageFilter Self;
        typedef ImageToImageFilter<TImageIn, TImageOut> Superclass;
        typedef SmartPointer<Self> Pointer;

        /** Needed for threads */
        typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(CalculatorT1ImageFilter, ImageToImageFilter);

        typedef typename TImageIn::PixelType  PixelTypeIn;
        typedef typename TImageOut::PixelType PixelTypeOut;

        void SetInputMagImage(const TImageIn *magImage);
        void SetInputPhaImage(const TImageIn *phaImage);

        TImageOut* GetAImage();
        TImageOut* GetBImage();
        TImageOut* GetT1starImage();
        TImageOut* GetT1Image();
        TImageOut* GetR2Image();
        TImageOut* GetChiSqrtImage();
        TImageOut* GetSNRImage();
        TImageOut* GetNShmolliSamplesUsedImage();
        TImageOut* GetSD_AImage();
        TImageOut* GetSD_BImage();
        TImageOut* GetSD_T1Image();

        /** Getters and setters. */
        void SetCalculator(Ox::CalculatorT1<PixelTypeIn>* calculator){
            this->m_Calculator = calculator;
        }

        itkSetMacro( LimitOutputIntensity, bool );
        itkGetMacro( LimitOutputIntensity, bool );
        itkBooleanMacro(LimitOutputIntensity);

        itkSetMacro( UpperLimitOutputIntensity, PixelTypeOut );
        itkGetMacro( UpperLimitOutputIntensity, PixelTypeOut );

        itkSetMacro( LowerLimitOutputIntensity, PixelTypeOut );
        itkGetMacro( LowerLimitOutputIntensity, PixelTypeOut );

    protected:
        /** Constructor. */
        CalculatorT1ImageFilter();

        /** Destructor. */
        ~CalculatorT1ImageFilter() {};

        /** Does the real work in threading */
        virtual void ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, ThreadIdType threadId ) ITK_OVERRIDE;

        /** Configure threads */
        virtual void BeforeThreadedGenerateData() ITK_OVERRIDE;

        /** handle dimensions difference between 3d input and 2d output */
        virtual void GenerateOutputInformation() ITK_OVERRIDE;

        /** Regions handling. */
        virtual const ImageRegionSplitterBase* GetImageRegionSplitter() const ITK_OVERRIDE{
            return m_ImageRegionSplitter;
        };

        /** Regions handling. */
        typename ImageRegionSplitterDirection::Pointer m_ImageRegionSplitter;

        /** Enforce maximum and minimum output value. */
        typename TImageOut::PixelType LimitResult( typename TImageOut::PixelType result );

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(CalculatorT1ImageFilter); //purposely not implemented

        /** Member variables. */
        Ox::CalculatorT1<PixelTypeIn>* m_Calculator;

        bool m_LimitOutputIntensity;
        PixelTypeOut m_UpperLimitOutputIntensity;
        PixelTypeOut m_LowerLimitOutputIntensity;
    };
} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCalculatorT1ImageFilter.hxx"
#endif

#endif // USE_ITK

#endif // itkCalculatorT1ImageFilter_h
