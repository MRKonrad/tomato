//
//  itkColorbar2DImageFilter
//  OxShmolliLib
//
//  Created by Konrad Werys on 13/7/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#ifndef OXSHMOLLILIB_ITKColorbar2DImageFilter_H
#define OXSHMOLLILIB_ITKColorbar2DImageFilter_H

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace itk {
    template<typename TImage>
    class Colorbar2DImageFilter : public ImageToImageFilter<TImage, TImage> {

    public:
        /** Standard class typedefs. */
        typedef Colorbar2DImageFilter Self;
        typedef ImageToImageFilter<TImage, TImage> Superclass;
        typedef SmartPointer<Self> Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(OxColorbarImageFilter, ImageToImageFilter);

        typedef typename TImage::PixelType PixelTypeIn;
        typedef typename TImage::PixelType PixelTypeOut;

    protected:
        /** Constructor. */
        Colorbar2DImageFilter() {};

        /** Destructor. */
        ~Colorbar2DImageFilter() {};

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(Colorbar2DImageFilter); //purposely not implemented

    };
} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkColorbar2DImageFilter.txx"
#endif

#endif //OXSHMOLLILIB_ITKColorbar2DImageFilter_H
