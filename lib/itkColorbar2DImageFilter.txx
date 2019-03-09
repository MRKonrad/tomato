//
//  itkColorbar2DImageFilter
//  TomatoLib
//
//  Created by Konrad Werys on 13/7/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#include "itkColorbar2DImageFilter.h"

namespace  itk {
    template<typename TImage>
    void
    Colorbar2DImageFilter<TImage>
    ::GenerateData() {

        typename TImage::ConstPointer input = this->GetInput();
        typename TImage::Pointer output = this->GetOutput();

        output->SetRegions(input->GetLargestPossibleRegion());
        output->Allocate();

        int nCols = output->GetLargestPossibleRegion().GetSize()[0];
        int nRows = output->GetLargestPossibleRegion().GetSize()[1];

        typedef itk::ImageRegionConstIteratorWithIndex<TImage> InputIteratorType;
        typedef itk::ImageRegionIteratorWithIndex<TImage> OutputIteratorType;

        typename TImage::IndexType idx;

        InputIteratorType iteratorInput(input, input->GetRequestedRegion());
        OutputIteratorType iteratorOutput(output, output->GetRequestedRegion());

        iteratorInput.GoToBegin();
        iteratorOutput.GoToBegin();

        while (!iteratorInput.IsAtEnd()) {

            idx = iteratorInput.GetIndex();
            //std::cout << idx << std::endl;

            iteratorOutput.Set(input->GetPixel(idx));

            if (idx[0] >= (int) (0.94 * nCols)) {
                if (idx[0] < (int) (0.96 * nCols)) {
                    iteratorOutput.Set(0);
                } else {
                    int value = (int)(4096.0 * (1. - ((double)idx[1] / (double)nRows)));
                    if (value > 4000) value = 4000;
                    if (GetZerosInsteadOfColorbar()) value = 0;

                    iteratorOutput.Set(value);
                }
            }
            for (long frefline = 500; frefline < 4001; frefline += 500) {
                if (idx[0] >= (int) (0.96 * nCols)
                    && idx[0] <= (int) (0.98 * nCols)
                    && idx[1] == (int) ((1. - frefline / 4096.) * nRows)) {

                    iteratorOutput.Set(0);
                }
                if (idx[0] >= (int) (0.95 * nCols)
                    && idx[0] <= (int) (0.96 * nCols)
                    && idx[1] == (int) ((1. - frefline / 4096.) * nRows)) {

                    int value = 4000;
                    if (GetZerosInsteadOfColorbar()) value = 0;

                    iteratorOutput.Set(value);
                }
            }

            ++iteratorInput;
            ++iteratorOutput;
        }
    }
} // end namespace
