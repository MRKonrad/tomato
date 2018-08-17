//
//  itkOxSortInvTimesImageFilter
//  OxShmolliLib
//
//  Created by Konrad Werys on 5/6/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#ifndef OXSHMOLLILIB_ITKOXSORTINVTIMESIMAGEFILTER_H
#define OXSHMOLLILIB_ITKOXSORTINVTIMESIMAGEFILTER_H

#include "CmakeConfigForOxShmolli2.h"
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

        itkSetMacro( InvTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( InvTimesNonSorted, vnl_vector<PixelTypeIn> );
        //itkSetMacro( InvTimesSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( InvTimesSorted, vnl_vector<PixelTypeIn> );

        itkSetMacro( RepTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RepTimesNonSorted, vnl_vector<PixelTypeIn> );
        //itkSetMacro( RepTimesSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RepTimesSorted, vnl_vector<PixelTypeIn> );

        itkSetMacro( RelAcqTimesNonSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RelAcqTimesNonSorted, vnl_vector<PixelTypeIn> );
        //itkSetMacro( RelAcqTimesSorted, vnl_vector<PixelTypeIn> );
        itkGetMacro( RelAcqTimesSorted, vnl_vector<PixelTypeIn> );

        //itkSetMacro( Indices, vnl_vector<int> );
        itkGetMacro( Indices, vnl_vector<int> );

    protected:
        /** Constructor. */
        SortInvTimesImageFilter() {};

        /** Destructor. */
        ~SortInvTimesImageFilter() {};

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;
    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(SortInvTimesImageFilter); //purposely not implemented

        vnl_vector<typename TImageIn::PixelType> m_InvTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_InvTimesSorted;
        vnl_vector<typename TImageIn::PixelType> m_RepTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_RepTimesSorted;
        vnl_vector<typename TImageIn::PixelType> m_RelAcqTimesNonSorted;
        vnl_vector<typename TImageIn::PixelType> m_RelAcqTimesSorted;
        vnl_vector<int> m_Indices;
    };

} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSortInvTimesImageFilter.txx"
#endif

#endif //USE_ITK

#endif //OXSHMOLLILIB_ITKOXSORTINVTIMESIMAGEFILTER_H
