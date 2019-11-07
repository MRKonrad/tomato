//
//  itkOxSortInvTimesImageFilter
//  TomatoLib
//
//  Created by Konrad Werys on 5/6/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#ifndef TomatoLIB_ITKOXSORTINVTIMESIMAGEFILTER_H
#define TomatoLIB_ITKOXSORTINVTIMESIMAGEFILTER_H

#include "tomatolib_export.h"
#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

#include <vnl/vnl_index_sort.h>

#include "itkImageToImageFilter.h"
#include "itkImageLinearIteratorWithIndex.h"

namespace itk {
    template<typename TImageIn, typename TImageOut>
    class SortInvTimesImageFilter : public ImageToImageFilter<TImageIn, TImageOut> {
    public:
        /** Standard class typedefs. */
        typedef SortInvTimesImageFilter Self;
        typedef ImageToImageFilter<TImageIn, TImageOut> Superclass;
        typedef SmartPointer<Self> Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(OxSortInvTimesImageFilter, ImageToImageFilter);

        typedef typename TImageIn::PixelType  PixelTypeIn;
        typedef typename TImageOut::PixelType PixelTypeOut;

        void SortIndices(vnl_vector<PixelTypeIn> vector);

        void SortByInvTimes();
        void SortByEchoTimes();
        void SortByRepTimes();
        void SortByTriggerTimes();
        void SortByAcqTimes();
        void SortByRelAcqTimes();

        itkSetMacro( InvTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( InvTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( InvTimesSorted, vnl_vector<PixelTypeIn> );

        itkSetMacro( EchoTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( EchoTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( EchoTimesSorted, vnl_vector<PixelTypeIn> );

        itkSetMacro( RepTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RepTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RepTimesSorted, vnl_vector<PixelTypeIn> );

        itkSetMacro( TriggerTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( TriggerTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( TriggerTimesSorted, vnl_vector<PixelTypeIn> );

        itkSetMacro( AcqTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( AcqTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( AcqTimesSorted, vnl_vector<PixelTypeIn> );

        itkSetMacro( RelAcqTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RelAcqTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RelAcqTimesSorted, vnl_vector<PixelTypeIn> );

        itkGetMacro( Indices, vnl_vector<int> );

    protected:
        /** Constructor. */
        SortInvTimesImageFilter() {
            m_nSamples = 0;
        };

        /** Destructor. */
        ~SortInvTimesImageFilter() {};

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;
    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(SortInvTimesImageFilter); //purposely not implemented

        vnl_vector<typename TImageIn::PixelType> m_InvTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_InvTimesSorted;
        vnl_vector<typename TImageIn::PixelType> m_EchoTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_EchoTimesSorted;
        vnl_vector<typename TImageIn::PixelType> m_RepTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_RepTimesSorted;
        vnl_vector<typename TImageIn::PixelType> m_TriggerTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_TriggerTimesSorted;
        vnl_vector<typename TImageIn::PixelType> m_AcqTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_AcqTimesSorted;
        vnl_vector<typename TImageIn::PixelType> m_RelAcqTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_RelAcqTimesSorted;
        vnl_vector<int> m_Indices;
        size_t m_nSamples;
    };

} //namespace ITK

#ifndef TOMATOLIB_COMPILED
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSortInvTimesImageFilter.txx"
#endif //ITK_MANUAL_INSTANTIATION
#endif //TOMATOLIB_COMPILED

#endif //USE_ITK

#endif //TomatoLIB_ITKOXSORTINVTIMESIMAGEFILTER_H
