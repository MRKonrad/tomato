/*!
 * \file itkCalculatorT1ImageFilter.h
 * \author Konrad Werys
 * \date 2018/08/13
 */

#ifndef itkCalculatorT1ImageFilter_h
#define itkCalculatorT1ImageFilter_h

#include "tomatolib_export.h"
#include "CmakeConfigForTomato.h"

#ifdef USE_ITK

#include "itkLogger.h"
#include "KWUtil.h"
#include "OxModel.h"
#include "OxFitter.h"
#include "OxCalculator.h"

#include "itkDataObject.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionSplitterDirection.h"
#include "itkImageRegionIterator.h"
#include "itkImageLinearIteratorWithIndex.h"


namespace itk {

    /**
     * \class CalculatorT1ImageFilter
     * @tparam TImageIn
     * @tparam TImageOut1
     */
    template<typename TImageIn, typename TImageOut1, typename TImageOut2 = TImageIn>
    class CalculatorT1ImageFilter : public ImageToImageFilter<TImageIn, TImageOut1> {
    public:
        /** Standard class typedefs. */
        typedef CalculatorT1ImageFilter Self;
        typedef ImageToImageFilter<TImageIn, TImageOut1> Superclass;
        typedef SmartPointer<Self> Pointer;

        /** Needed for threads */
        typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(CalculatorT1ImageFilter, ImageToImageFilter);

        typedef typename TImageIn::PixelType  PixelTypeIn;
        typedef typename TImageOut1::PixelType PixelTypeOut;

        void SetInputMagImage(const TImageIn *magImage);
        void SetInputPhaImage(const TImageIn *phaImage);

        TImageOut2* GetMagSignRecovered();

        TImageOut1* GetAImage();
        TImageOut1* GetBImage();
        TImageOut1* GetT1starImage();
        TImageOut1* GetT1Image();
        TImageOut1* GetR2Image();
        TImageOut1* GetChiSqrtImage();
        TImageOut1* GetSNRImage();
        TImageOut1* GetNShmolliSamplesUsedImage();
        TImageOut1* GetSD_AImage();
        TImageOut1* GetSD_BImage();
        TImageOut1* GetSD_T1Image();
        TImageOut1* GetT2Image();

        /** Getters and setters. */
        void SetCalculator(Ox::Calculator<PixelTypeIn>* calculator){
            this->m_Calculator = calculator;
        }

        Ox::Calculator<PixelTypeIn> *GetCalculator() const{
            return m_Calculator;
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
        typename TImageOut1::PixelType LimitResult(typename TImageOut1::PixelType result );

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(CalculatorT1ImageFilter); //purposely not implemented

        /** Member variables. */
        Ox::Calculator<PixelTypeIn>* m_Calculator;

        bool m_LimitOutputIntensity;
        PixelTypeOut m_UpperLimitOutputIntensity;
        PixelTypeOut m_LowerLimitOutputIntensity;
    };
} //namespace ITK

#ifndef TOMATOLIB_COMPILED
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCalculatorT1ImageFilter.hxx"
#endif //ITK_MANUAL_INSTANTIATION
#endif //TOMATOLIB_COMPILED

#endif // USE_ITK

#endif // itkCalculatorT1ImageFilter_h
