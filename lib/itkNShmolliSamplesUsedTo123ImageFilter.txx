//
//  NShmolliSamplesUsedTo123ImageFilter
//  TomatoLib
//
//  Created by Konrad Werys on 24/11/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#include "itkNShmolliSamplesUsedTo123ImageFilter.h"

namespace  itk {
    template<typename TImage>
    void
    NShmolliSamplesUsedTo123ImageFilter<TImage>
    ::GenerateData() {

        typename TImage::ConstPointer input = this->GetInput();
        typename TImage::Pointer output = this->GetOutput();

        output->SetRegions(input->GetLargestPossibleRegion());
        output->Allocate();

        typedef itk::ImageRegionConstIterator< TImage > InputIteratorType;
        typedef itk::ImageRegionIterator< TImage> OutputIteratorType;

        InputIteratorType iteratorInput(input, input->GetRequestedRegion());
        OutputIteratorType iteratorOutput(output, output->GetRequestedRegion());

        iteratorInput.GoToBegin();
        iteratorOutput.GoToBegin();

        PixelTypeIn temp;
        while (!iteratorInput.IsAtEnd()) {
            temp = iteratorInput.Get();

            if (temp == 0) {
                iteratorOutput.Set(0);
            } else {
                iteratorOutput.Set((temp - 4) * 1000);
            }

            ++iteratorInput;
            ++iteratorOutput;
        }
    }
} // end namespace
