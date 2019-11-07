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
    ::SortIndices(vnl_vector<PixelTypeIn> vector) {
        m_nSamples = vector.size();

        if (m_nSamples < 1){
            throw std::runtime_error("SortInvTimesImageFilter::GenerateData: Trying to sort based on empty vector");
        }

        vnl_vector<typename TImageIn::PixelType> temp;
        temp.set_size(m_nSamples);
        temp.fill(0);

        m_Indices.set_size(m_nSamples);
        for (size_t i = 0; i < m_nSamples; ++i) {
            m_Indices[i] = i;
        }

        vnl_index_sort<PixelTypeIn,int> vnlSort;
        vnlSort.vector_sort(vector, temp, m_Indices);
    }

    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::SortByInvTimes() {
        SortIndices(m_InvTimesNonSorted);
    }

    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::SortByEchoTimes() {
        SortIndices(m_EchoTimesNonSorted);
    }

    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::SortByRepTimes() {
        SortIndices(m_RepTimesNonSorted);
    }

    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::SortByTriggerTimes() {
        SortIndices(m_TriggerTimesNonSorted);
    }

    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::SortByAcqTimes() {
        SortIndices(m_AcqTimesNonSorted);
    }

    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::SortByRelAcqTimes() {
        SortIndices(m_RelAcqTimesNonSorted);
    }

    template<typename TImageIn, typename TImageOut>
    void
    SortInvTimesImageFilter<TImageIn, TImageOut>
    ::GenerateData() {

        if (m_nSamples < 1){
            throw std::runtime_error("SortInvTimesImageFilter::GenerateData Have you called the sorting method like SortByInvTimes?");
        }

        // 1. apply new indices to invTimes vector
        if (m_InvTimesNonSorted.size() == m_nSamples) {
            m_InvTimesSorted.set_size(m_nSamples);
            m_InvTimesSorted.fill(0);
            for (size_t i = 0; i < m_nSamples; ++i) {
                m_InvTimesSorted[i] = m_InvTimesNonSorted[m_Indices[i]];
            }
        }

        // 2. apply new indices to echoTimes vector
        if (m_EchoTimesNonSorted.size() == m_nSamples) {
            m_EchoTimesSorted.set_size(m_nSamples);
            m_EchoTimesSorted.fill(0);
            for (size_t i = 0; i < m_nSamples; ++i) {
                m_EchoTimesSorted[i] = m_EchoTimesNonSorted[m_Indices[i]];
            }
        }

        // 3. apply new indices to repTimes vector
        if (m_RepTimesNonSorted.size() == m_nSamples) {
            m_RepTimesSorted.set_size(m_nSamples);
            m_RepTimesSorted.fill(0);
            for (size_t i = 0; i < m_nSamples; ++i) {
                m_RepTimesSorted[i] = m_RepTimesNonSorted[m_Indices[i]];
            }
        }

        // 4. apply new indices to TriggerTimes vector
        if (m_TriggerTimesNonSorted.size() == m_nSamples) {
            m_TriggerTimesSorted.set_size(m_nSamples);
            m_TriggerTimesSorted.fill(0);
            for (size_t i = 0; i < m_nSamples; ++i) {
                m_TriggerTimesSorted[i] = m_TriggerTimesNonSorted[m_Indices[i]];
            }
        }

        // 5. apply new indices to acqTimes vector
        if (m_AcqTimesNonSorted.size() == m_nSamples) {
            m_AcqTimesSorted.set_size(m_nSamples);
            m_AcqTimesSorted.fill(0);
            for (size_t i = 0; i < m_nSamples; ++i) {
                m_AcqTimesSorted[i] = m_AcqTimesNonSorted[m_Indices[i]];
            }
        }

        // 6. apply new indices to RelAcqTimes vector
        if (m_RelAcqTimesNonSorted.size() == m_nSamples) {
            m_RelAcqTimesSorted.set_size(m_nSamples);
            m_RelAcqTimesSorted.fill(0);
            for (size_t i = 0; i < m_nSamples; ++i) {
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
