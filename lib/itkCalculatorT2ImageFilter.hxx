/*!
 * \file itkCalculatorT2ImageFilter.hxx
 * \author Konrad Werys
 * \date 2019/11/08
 */

#ifndef itkCalculatorT2ImageFilter_txx
#define itkCalculatorT2ImageFilter_txx

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

namespace itk {

    template< typename TImageIn, typename TImageOut >
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::CalculatorT2ImageFilter() {

        this->m_LimitOutputIntensity = true;
        this->m_UpperLimitOutputIntensity = 4000;
        this->m_LowerLimitOutputIntensity = 0;

        this->SetNumberOfRequiredInputs(1);
        this->SetNumberOfRequiredOutputs(4);
        for (unsigned i = 0; i < this->GetNumberOfRequiredOutputs(); ++i) {
            this->SetNthOutput(i, (TImageOut::New()).GetPointer());
        }

        this->m_ImageRegionSplitter = ImageRegionSplitterDirection::New();
        this->m_ImageRegionSplitter->SetDirection(2);

    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::SetInputMagImage(const TImageIn *image) {
        this->SetNthInput(0, const_cast<TImageIn *>(image));
    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::BeforeThreadedGenerateData() {

        printf("Number of threads: %d. ", this->GetNumberOfThreads());

        if(!m_Calculator) throw itk::ExceptionObject(__FILE__, __LINE__, "Set the Calculator!");

        printf("\n");
    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::GenerateOutputInformation(){
        typename TImageIn::ConstPointer imageMag = this->GetInput(0);
        typename TImageIn::RegionType magImageRegion = imageMag->GetLargestPossibleRegion();

        typename TImageOut::IndexType start;
        start[0] = magImageRegion.GetIndex()[0];
        start[1] = magImageRegion.GetIndex()[1];

        typename TImageOut::SizeType size;
        size[0] = magImageRegion.GetSize()[0];
        size[1] = magImageRegion.GetSize()[1];

        typename TImageOut::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);

        typename TImageOut::SpacingType spacing;
        spacing[0] = imageMag->GetSpacing()[0];
        spacing[1] = imageMag->GetSpacing()[1];

        typename TImageOut::PointType origin;
        origin[0] = imageMag->GetOrigin()[0];
        origin[1] = imageMag->GetOrigin()[1];

        typename TImageOut::DirectionType direction;

        direction[0][0] = imageMag->GetDirection()[0][0];
        direction[0][1] = imageMag->GetDirection()[0][1];
        direction[1][0] = imageMag->GetDirection()[1][0];
        direction[1][1] = imageMag->GetDirection()[1][1];

        for (unsigned int i = 0; i < this->GetNumberOfOutputs(); i++){
            this->GetOutput(i)->SetLargestPossibleRegion(region);
            this->GetOutput(i)->SetSpacing(spacing);
            this->GetOutput(i)->SetOrigin(origin);
            this->GetOutput(i)->SetDirection(direction);
        }
    }

    template<typename TImageIn, typename TImageOut>
    typename TImageOut::PixelType
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::LimitResult( typename TImageOut::PixelType result ) {
        if (GetLimitOutputIntensity()) {
            if (result < m_LowerLimitOutputIntensity) {
                result = m_LowerLimitOutputIntensity;
            }
            if (result > m_UpperLimitOutputIntensity) {
                result = m_UpperLimitOutputIntensity;
            }
        }
        return result;
    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, ThreadIdType threadId ){

        typename TImageIn::IndexType idx;

        typename TImageIn::ConstPointer imageMag = this->GetInput(0);

        std::vector<typename TImageOut::Pointer> outputImagesVector;
        for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i){
            outputImagesVector.push_back(this->GetOutput(i));
        }

        typename TImageIn::SizeType magImageSize = imageMag->GetLargestPossibleRegion().GetSize();

        typename TImageIn::SizeType inputSize = imageMag->GetLargestPossibleRegion().GetSize();
        inputSize[0] = outputRegionForThread.GetSize()[0];
        inputSize[1] = outputRegionForThread.GetSize()[1];

        typename TImageIn::IndexType inputIndex = imageMag->GetLargestPossibleRegion().GetIndex();;
        inputIndex[0] = outputRegionForThread.GetIndex()[0];
        inputIndex[1] = outputRegionForThread.GetIndex()[1];

        typename TImageIn::RegionType inputRegion = imageMag->GetLargestPossibleRegion();
        inputRegion.SetSize(inputSize);
        inputRegion.SetIndex(inputIndex);

        typedef itk::ImageLinearConstIteratorWithIndex< TImageIn > InputIteratorType;
        typedef itk::ImageRegionIterator< TImageOut > OutputIteratorType;

        InputIteratorType  itMag( imageMag, inputRegion );

        std::vector<OutputIteratorType> itOutVector;
        for (unsigned int i = 0; i < outputImagesVector.size(); ++i){
            itOutVector.push_back(OutputIteratorType(outputImagesVector[i], outputRegionForThread));
        }

        itMag.SetDirection( 2 ); // Walk along third dimension it.GoToBegin();

        /**
         * Each thread needs its own calculator object, so
         * clone the calculator and the contents!
         */
        int nSamples = magImageSize[2];
        Ox::CalculatorT2<PixelTypeIn> *calculator = m_Calculator->newByCloning();
        Ox::ModelT2<PixelTypeIn> *functionsObject = m_Calculator->getModelT2()->newByCloning();
        Ox::Fitter<PixelTypeIn> *fitter = m_Calculator->getFitter()->newByCloning();
        Ox::SignCalculator<PixelTypeIn> *signCalculator = m_Calculator->getSignCalculator()->newByCloning();
        Ox::StartPointCalculator<PixelTypeIn> *startPointCalculator = m_Calculator->getStartPointCalculator()->newByCloning();

        // configure
        calculator->setModelT2(functionsObject);
        calculator->setFitter(fitter);
        calculator->setSignCalculator(signCalculator);
        calculator->setStartPointCalculator(startPointCalculator);
        calculator->setNSamples(nSamples);
        calculator->setInvTimes(m_Calculator->getInvTimes());
        calculator->setEchoTimes(m_Calculator->getEchoTimes());

        // set the thread id in the minimiser
        fitter->setThreadId(threadId);

        // move output iterators
        for (unsigned int i = 0; i < outputImagesVector.size(); ++i) {
            itOutVector[i].GoToBegin();
        }

        // init alloc
        PixelTypeIn *sigMag = new PixelTypeIn[nSamples];

        while( !itMag.IsAtEnd() ) {
            // move input iterators
            itMag.GoToBeginOfLine();

            // get mag from the iterators
            while ( !itMag.IsAtEndOfLine() ) {
                // get index
                idx = itMag.GetIndex();

                // get mag
                sigMag[idx[2]] = itMag.Get();

                // move iterators
                ++itMag;
            }

            // set Mag
            calculator->setSigMag(sigMag);

            // calculate
            calculator->prepareToCalculate();
            calculator->calculate();

            // to add/remove output
            // 1. change this->SetNumberOfRequiredOutputs(XXX); in the constructor
            // 2. add GetXXXImage method. Numbers below have to agree with GetXXXImage methods
            itOutVector[0].Set(LimitResult(calculator->getResults().T2));
            itOutVector[1].Set(LimitResult(calculator->getResults().R2));
            itOutVector[2].Set(LimitResult(calculator->getResults().A));
            itOutVector[3].Set(LimitResult(calculator->getResults().B));

            // iterate all iterators accordingly
            for (unsigned int i = 0; i < outputImagesVector.size(); ++i) {
                ++itOutVector[i];
            }
            itMag.NextLine();
        }

        // cleanup
        delete functionsObject;
        delete fitter;
        delete signCalculator;
        delete startPointCalculator;
        delete calculator;
        delete [] sigMag;
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::GetT2Image() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(0) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::GetR2Image() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(1) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::GetAImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(2) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT2ImageFilter<TImageIn, TImageOut>
    ::GetBImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(3) );
    }

}// end namespace itk

#endif // USE_ITK

#endif //itkCalculatorT2ImageFilter_txx