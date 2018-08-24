/*!
 * \file itkCalculatorT1ImageFilter.hxx
 * \author Konrad Werys
 * \date 2018/08/13
 */

#ifndef itkCalculatorT1ImageFilter_txx
#define itkCalculatorT1ImageFilter_txx

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

namespace itk {

    template< typename TImageIn, typename TImageOut >
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::CalculatorT1ImageFilter() {

        //this->UseShmolliOn();
        this->m_LimitOutputIntensity = true;
        this->m_UpperLimitOutputIntensity = 4000;
        this->m_LowerLimitOutputIntensity = 0;

        this->SetNumberOfRequiredInputs(1);
        this->SetNumberOfRequiredOutputs(11);
        for (unsigned i = 0; i < this->GetNumberOfRequiredOutputs(); ++i) {
            this->SetNthOutput(i, (TImageOut::New()).GetPointer());
        }

        this->m_ImageRegionSplitter = ImageRegionSplitterDirection::New();
        this->m_ImageRegionSplitter->SetDirection(2);

//        this->SetCoordinateTolerance(1e-5);
//        this->SetDirectionTolerance(1e-5);
//        std::cout << this->GetCoordinateTolerance() << std::endl;
    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::SetInputMagImage(const TImageIn *image) {
        this->SetNthInput(0, const_cast<TImageIn *>(image));
    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::SetInputPhaImage(const TImageIn *image) {
        this->SetNthInput(1, const_cast<TImageIn *>(image));
    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::BeforeThreadedGenerateData() {

        printf("Number of threads: %d. ", this->GetNumberOfThreads());

        if(!m_Calculator) throw itk::ExceptionObject(__FILE__, __LINE__, "Set the Calculator!");

        printf("\n");
    }

    template<typename TImageIn, typename TImageOut>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut>
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

        // TODO output has only 2d origin and spacing. How to deal with it?
        typename TImageOut::SpacingType spacing;
        spacing[0] = imageMag->GetSpacing()[0];
        spacing[1] = imageMag->GetSpacing()[1];

//        imageA->SetOrigin(origin);
//        imageA->SetSpacing(spacing);
//        imageB->SetOrigin(origin);
//        imageB->SetSpacing(spacing);

        for (unsigned int i = 0; i < this->GetNumberOfOutputs(); i++){
            this->GetOutput(i)->SetLargestPossibleRegion(region);
            this->GetOutput(i)->SetSpacing(spacing);
        }
    }

    template<typename TImageIn, typename TImageOut>
    typename TImageOut::PixelType
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::LimitResult( typename TImageOut::PixelType result ) {
        //typename TImageOut::InputPixelType result = std::round(resultBeforeCasting);
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
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, ThreadIdType threadId ){

        //std::cout << threadId << std::endl;
        //std::cout << outputRegionForThread << std::endl;

        typename TImageIn::IndexType idx;

        typename TImageIn::ConstPointer imageMag = this->GetInput(0);
        typename TImageIn::ConstPointer imagePha = this->GetInput(1);

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
        InputIteratorType  itPha;

        if (imagePha) {
            InputIteratorType temp( imagePha, inputRegion );
            itPha = temp;
        }

        std::vector<OutputIteratorType> itOutVector;
        for (unsigned int i = 0; i < outputImagesVector.size(); ++i){
            itOutVector.push_back(OutputIteratorType(outputImagesVector[i], outputRegionForThread));
        }

        itMag.SetDirection( 2 ); // Walk along third dimension it.GoToBegin();
        if (imagePha) itPha.SetDirection( 2 ); // Walk along third dimension it.GoToBegin();

        /**
         * Each thread needs its own calculator object, so
         * clone the calculator and the contents!
         */
        int nSamples = magImageSize[2];
        Ox::CalculatorT1<PixelTypeIn> *calculator = m_Calculator->newByCloning();
        Ox::FunctionsT1<PixelTypeIn> *functionsObject = m_Calculator->getFunctionsT1()->newByCloning();
        Ox::Fitter<PixelTypeIn> *fitter = m_Calculator->getFitter()->newByCloning();
        Ox::SignCalculator<PixelTypeIn> *signCalculator = m_Calculator->getSignCalculator()->newByCloning();
        Ox::StartPointCalculator<PixelTypeIn> *startPointCalculator = m_Calculator->getStartPointCalculator()->newByCloning();

        // configure
        calculator->setFunctionsT1(functionsObject);
        calculator->setFitter(fitter);
        calculator->setSignCalculator(signCalculator);
        calculator->setStartPointCalculator(startPointCalculator);
        calculator->setNSamples(nSamples);
        calculator->setInvTimes(m_Calculator->getInvTimes());

        // set the thread id in the minimiser
        fitter->setThreadId(threadId);

//        // make sure calculator is ready to calculate
//        calculator->VerifyReadyToCalculate();

        // move output iterators
        for (unsigned int i = 0; i < outputImagesVector.size(); ++i) {
            itOutVector[i].GoToBegin();
        }

        // init alloc
        PixelTypeIn *sigMag = new PixelTypeIn[nSamples];
        PixelTypeIn *sigPha = new PixelTypeIn[nSamples];

        while( !itMag.IsAtEnd() ) {
            // move input iterators
            itMag.GoToBeginOfLine();
            // itPha.GoToBeginOfLine();
            if (imagePha) itPha.GoToBeginOfLine();

            // get mag and phase from the iterators
            while ( !itMag.IsAtEndOfLine() ) {
                // get index
                idx = itMag.GetIndex();

                // get mag and phase
                sigMag[idx[2]] = itMag.Get();
                if (imagePha) sigPha[idx[2]] = itPha.Get();

                // move iterators
                ++itMag;
                if (imagePha) ++itPha;
            }

            // set Mag and Phase
            calculator->setSigMag(sigMag);
            calculator->setSigPha(sigPha);

            // calculate
            calculator->prepareToCalculate();
            calculator->calculate();

            // to add/remove output
            // 1. change this->SetNumberOfRequiredOutputs(XXX); in the constructor
            // 2. add GetXXXImage method. Numbers below have to agree with GetXXXImage methods
            itOutVector[0].Set(LimitResult(calculator->getResults().T1));
            itOutVector[1].Set(LimitResult(calculator->getResults().R2));
            itOutVector[2].Set(LimitResult(calculator->getResults().A));
            itOutVector[3].Set(LimitResult(calculator->getResults().B));
            itOutVector[4].Set(LimitResult(calculator->getResults().T1star));
            itOutVector[5].Set(LimitResult(calculator->getResults().ChiSqrt));
            itOutVector[6].Set(LimitResult(calculator->getResults().SNR));
            itOutVector[7].Set(LimitResult(calculator->getResults().NShmolliSamplesUsed));
            itOutVector[8].Set(LimitResult(calculator->getResults().SD_A));
            itOutVector[9].Set(LimitResult(calculator->getResults().SD_B));
            itOutVector[10].Set(LimitResult(calculator->getResults().SD_T1));

            // iterate all iterators accordingly
            for (unsigned int i = 0; i < outputImagesVector.size(); ++i) {
                ++itOutVector[i];
            }
            itMag.NextLine();
            if (imagePha) itPha.NextLine();
        }

        // cleanup
        delete functionsObject;
        delete fitter;
        delete signCalculator;
        delete startPointCalculator;
        delete calculator;
        delete [] sigMag;
        delete [] sigPha;
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetT1Image() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(0) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetR2Image() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(1) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetAImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(2) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetBImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(3) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetT1starImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(4) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetChiSqrtImage(){
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(5) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetSNRImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(6) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetNShmolliSamplesUsedImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(7) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetSD_AImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(8) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetSD_BImage() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(9) );
    }

    template< typename TImageIn, typename TImageOut >
    TImageOut*
    CalculatorT1ImageFilter<TImageIn, TImageOut>
    ::GetSD_T1Image() {
        return dynamic_cast< TImageOut * >(
                this->ProcessObject::GetOutput(10) );
    }

}// end namespace itk

#endif // USE_ITK

#endif //itkCalculatorT1ImageFilter_txx