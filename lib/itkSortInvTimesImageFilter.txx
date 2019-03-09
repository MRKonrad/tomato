//
//  itkOxSortInvTimesImageFilter
//  TomatoLib
//
//  Created by Konrad Werys on 5/6/17.
//  Copyright © 2017 Konrad Werys. All rights reserved.
//

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

namespace  itk {
    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::GenerateData() {

        size_t nSamples = m_InvTimesNonSorted.size();

        if (nSamples < 1){
            throw std::runtime_error("Trying to sort based on empty InvTimes vector");
        }

        // initialize values
        m_InvTimesSorted.set_size(nSamples);
        m_InvTimesSorted.fill(0);

        m_Indices.set_size(nSamples);
        for (int i = 0; i < nSamples; ++i) {
            m_Indices[i] = i;
        }

        // 1. sort invTimes vector
        vnl_index_sort<PixelTypeIn,int> vnlSort;
        vnlSort.vector_sort(m_InvTimesNonSorted, m_InvTimesSorted, m_Indices);

        // 2. apply new idices to repTimes vector
        if (m_RepTimesNonSorted.size() == m_InvTimesNonSorted.size()) {
            m_RepTimesSorted.set_size(nSamples);
            m_RepTimesSorted.fill(0);
            for (int i = 0; i < nSamples; ++i) {
                m_RepTimesSorted[i] = m_RepTimesNonSorted[m_Indices[i]];
            }
        }

        // 3. apply new idices to acqTimes vector
        if (m_RelAcqTimesNonSorted.size() == m_InvTimesNonSorted.size()) {
            m_RelAcqTimesSorted.set_size(nSamples);
            m_RelAcqTimesSorted.fill(0);
            for (int i = 0; i < nSamples; ++i) {
                m_RelAcqTimesSorted[i] = m_RelAcqTimesNonSorted[m_Indices[i]];
            }
        }

        // 4. set/get iterator with certain indices
        typename TImageIn::ConstPointer input = this->GetInput();
        typename TImageOut::Pointer output = this->GetOutput();

        output->SetRegions(input->GetLargestPossibleRegion());
        output->Allocate();

        typedef itk::ImageLinearConstIteratorWithIndex<TImageIn> InputIteratorType;
        typedef itk::ImageLinearIteratorWithIndex<TImageOut> OutputIteratorType;

        typename TImageIn::IndexType idxInput;
        typename TImageIn::IndexType idxOutput;

        InputIteratorType iteratorInput(input, input->GetRequestedRegion());
        OutputIteratorType iteratorOutput(output, output->GetRequestedRegion());

        iteratorInput.SetDirection(2);
        iteratorOutput.SetDirection(2);

        iteratorInput.GoToBegin();
        iteratorOutput.GoToBegin();

        while (!iteratorInput.IsAtEnd()) {

            iteratorInput.GoToBeginOfLine();
            iteratorOutput.GoToBeginOfLine();

            while (!iteratorOutput.IsAtEndOfLine()) {
                idxInput = iteratorInput.GetIndex();
                idxOutput = iteratorOutput.GetIndex();
                // change the order of 3rd dimension according to sorted invTimes
                idxInput[2] = idxInput[2] + m_Indices[idxOutput[2]];
                iteratorOutput.Set( input->GetPixel( idxInput ) );
                ++iteratorOutput;
            }

            iteratorInput.NextLine();
            iteratorOutput.NextLine();
        }
    }
} // end namespace

#endif // USE_ITK
