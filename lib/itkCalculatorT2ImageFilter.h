/*!
 * \file itkCalculatorT2ImageFilter.h
 * \author Konrad Werys
 * \date 2019/11/08
 */

#ifndef itkCalculatorT2ImageFilter_h
#define itkCalculatorT2ImageFilter_h

#include "tomatolib_export.h"
#include "CmakeConfigForTomato.h"

#ifdef USE_ITK

#include "itkLogger.h"
#include "KWUtil.h"
#include "OxFitter.h"
#include "OxCalculatorT2.h"

#include "itkDataObject.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionSplitterDirection.h"
#include "itkImageRegionIterator.h"
#include "itkImageLinearIteratorWithIndex.h"


namespace itk {

    /**
     * \class CalculatorT2ImageFilter
     * @tparam TImageIn
     * @tparam TImageOut
     */
    template<typename TImageIn, typename TImageOut>
    class CalculatorT2ImageFilter : public ImageToImageFilter<TImageIn, TImageOut> {
    public:
        /** Standard class typedefs. */
        typedef CalculatorT2ImageFilter Self;
        typedef ImageToImageFilter<TImageIn, TImageOut> Superclass;
        typedef SmartPointer<Self> Pointer;

        /** Needed for threads */
        typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(CalculatorT2ImageFilter, ImageToImageFilter);

        typedef typename TImageIn::PixelType  PixelTypeIn;
        typedef typename TImageOut::PixelType PixelTypeOut;

        void SetInputMagImage(const TImageIn *magImage);

        TImageOut* GetAImage();
        TImageOut* GetBImage();
        TImageOut* GetT2Image();
        TImageOut* GetR2Image();

        /** Getters and setters. */
        void SetCalculator(Ox::CalculatorT2<PixelTypeIn>* calculator){
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
        CalculatorT2ImageFilter();

        /** Destructor. */
        ~CalculatorT2ImageFilter() {};

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
        ITK_DISALLOW_COPY_AND_ASSIGN(CalculatorT2ImageFilter); //purposely not implemented

        /** Member variables. */
        Ox::CalculatorT2<PixelTypeIn>* m_Calculator;

        bool m_LimitOutputIntensity;
        PixelTypeOut m_UpperLimitOutputIntensity;
        PixelTypeOut m_LowerLimitOutputIntensity;
    };
} //namespace ITK

#ifndef TOMATOLIB_COMPILED
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCalculatorT2ImageFilter.hxx"
#endif //ITK_MANUAL_INSTANTIATION
#endif //TOMATOLIB_COMPILED

#endif // USE_ITK

#endif // itkCalculatorT2ImageFilter_h
