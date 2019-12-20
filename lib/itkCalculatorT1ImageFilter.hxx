/*!
 * \file itkCalculatorT1ImageFilter.hxx
 * \author Konrad Werys
 * \date 2018/08/13
 */

#ifndef itkCalculatorT1ImageFilter_txx
#define itkCalculatorT1ImageFilter_txx

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

namespace itk {

    template< typename TImageIn, typename TImageOut1, typename TImageOut2> 
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::CalculatorT1ImageFilter() {
        this->m_LimitOutputIntensity = true;
        this->m_UpperLimitOutputIntensity = 4000;
        this->m_LowerLimitOutputIntensity = 0;

        this->SetNumberOfRequiredInputs(1);
        this->SetNumberOfRequiredOutputs(12);
        for (unsigned i = 0; i < this->GetNumberOfRequiredOutputs(); ++i) {
            this->SetNthOutput(i, (TImageOut1::New()).GetPointer());
        }
        this->SetNumberOfRequiredOutputs(this->GetNumberOfRequiredOutputs() + 1);
        this->SetNthOutput(this->GetNumberOfRequiredOutputs()-1, (TImageOut2::New()).GetPointer());

        this->m_ImageRegionSplitter = ImageRegionSplitterDirection::New();
        this->m_ImageRegionSplitter->SetDirection(2);

    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::SetInputMagImage(const TImageIn *image) {
        this->SetNthInput(0, const_cast<TImageIn *>(image));
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::SetInputPhaImage(const TImageIn *image) {
        this->SetNthInput(1, const_cast<TImageIn *>(image));
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::BeforeThreadedGenerateData() {

        if(!m_Calculator) throw itk::ExceptionObject(__FILE__, __LINE__, "Set the Calculator!");

        printf("Number of threads: %d. ", this->GetNumberOfThreads());

        if (m_Calculator->getInvTimes()) {
            printf("\nInvTimes: ");
            for (size_t i = 0; i < m_Calculator->getNSamples(); i++) {
                printf("%.0f ", m_Calculator->getInvTimes()[i]);
            }
        }

        if (m_Calculator->getEchoTimes()) {
            printf("\nEchoTimes: ");
            for (size_t i = 0; i < m_Calculator->getNSamples(); i++) {
                printf("%.1f ", m_Calculator->getEchoTimes()[i]);
            }
        }

        printf("\n");
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GenerateOutputInformation(){
        typename TImageIn::ConstPointer imageMag = this->GetInput(0);
        typename TImageIn::RegionType magImageRegion = imageMag->GetLargestPossibleRegion();

        typename TImageOut1::IndexType start;
        start[0] = magImageRegion.GetIndex()[0];
        start[1] = magImageRegion.GetIndex()[1];

        typename TImageOut1::SizeType size;
        size[0] = magImageRegion.GetSize()[0];
        size[1] = magImageRegion.GetSize()[1];

        typename TImageOut1::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);

        typename TImageOut1::SpacingType spacing;
        spacing[0] = imageMag->GetSpacing()[0];
        spacing[1] = imageMag->GetSpacing()[1];

        typename TImageOut1::PointType origin;
        origin[0] = imageMag->GetOrigin()[0];
        origin[1] = imageMag->GetOrigin()[1];

        typename TImageOut1::DirectionType direction;

        direction[0][0] = imageMag->GetDirection()[0][0];
        direction[0][1] = imageMag->GetDirection()[0][1];
        direction[1][0] = imageMag->GetDirection()[1][0];
        direction[1][1] = imageMag->GetDirection()[1][1];

        // last output has different type and is configured below
        for (unsigned int i = 0; i < this->GetNumberOfOutputs()-1; i++){
            this->GetOutput(i)->SetRegions(region);
            this->GetOutput(i)->SetSpacing(spacing);
            this->GetOutput(i)->SetOrigin(origin);
            this->GetOutput(i)->SetDirection(direction);
        }

        // last output
        this->GetMagSignRecovered()->SetRegions(magImageRegion);
        this->GetMagSignRecovered()->SetSpacing(imageMag->GetSpacing());
        this->GetMagSignRecovered()->SetOrigin(imageMag->GetOrigin());
        this->GetMagSignRecovered()->SetDirection(imageMag->GetDirection());
        this->GetMagSignRecovered()->Allocate();
        this->GetMagSignRecovered()->FillBuffer(0);

    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    typename TImageOut1::PixelType
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::LimitResult( typename TImageOut1::PixelType result ) {

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

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    void
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, ThreadIdType threadId ){

        typename TImageIn::IndexType idx;

        typename TImageIn::ConstPointer imageMag = this->GetInput(0);
        typename TImageIn::ConstPointer imagePha = this->GetInput(1);

        std::vector<typename TImageOut1::Pointer> outputImagesVector;
        for (unsigned int i = 0; i < this->GetNumberOfOutputs()-1; ++i){
            outputImagesVector.push_back(this->GetOutput(i));
        }
        typename TImageOut2::Pointer imageMagSignRecov = this->GetMagSignRecovered();

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
        typedef itk::ImageRegionIterator< TImageOut1 > Output2dIteratorType;
        typedef itk::ImageLinearIteratorWithIndex< TImageIn > Output3dIteratorType;

        InputIteratorType  itMag( imageMag, inputRegion );
        InputIteratorType  itPha;

        if (imagePha) {
            InputIteratorType temp( imagePha, inputRegion );
            itPha = temp;
        }

        std::vector<Output2dIteratorType> itOutVector;
        for (unsigned int i = 0; i < outputImagesVector.size(); ++i){
            itOutVector.push_back(Output2dIteratorType(outputImagesVector[i], outputRegionForThread));
        }
        Output3dIteratorType itOutMagSignRecov(imageMagSignRecov, inputRegion);

        itMag.SetDirection( 2 ); // Walk along third dimension it.GoToBegin();
        if (imagePha) itPha.SetDirection( 2 ); // Walk along third dimension it.GoToBegin();
        itOutMagSignRecov.SetDirection(2);

        /**
         * Each thread needs its own calculator object, so
         * clone the calculator and the contents!
         */
        int nSamples = magImageSize[2];
        Ox::Calculator<PixelTypeIn> *calculator = m_Calculator->newByCloning();
        Ox::Model<PixelTypeIn> *functionsObject = m_Calculator->getModel()->newByCloning();
        Ox::Fitter<PixelTypeIn> *fitter = m_Calculator->getFitter()->newByCloning();
        Ox::SignCalculator<PixelTypeIn> *signCalculator = 0;
        if (m_Calculator->getSignCalculator()) {
            signCalculator = m_Calculator->getSignCalculator()->newByCloning();
        }
        Ox::StartPointCalculator<PixelTypeIn> *startPointCalculator = 0;
        if (m_Calculator->getStartPointCalculator()) {
            startPointCalculator = m_Calculator->getStartPointCalculator()->newByCloning();
        }

        // configure
        calculator->setModel(functionsObject);
        calculator->setFitter(fitter);
        calculator->setSignCalculator(signCalculator);
        calculator->setStartPointCalculator(startPointCalculator);
        calculator->setNSamples(nSamples);
        calculator->setInvTimes(m_Calculator->getInvTimes());
        calculator->setEchoTimes(m_Calculator->getEchoTimes());
        calculator->setNoise(m_Calculator->getNoise());

        // set the thread id in the minimiser
        fitter->setThreadId(threadId);

        // move output iterators
        for (unsigned int i = 0; i < outputImagesVector.size(); ++i) {
            itOutVector[i].GoToBegin();
        }
        itOutMagSignRecov.GoToBegin();

        // init alloc
        PixelTypeIn *sigMag = new PixelTypeIn[nSamples];
        PixelTypeIn *sigPha = new PixelTypeIn[nSamples];

        while( !itMag.IsAtEnd() ) {
            // move input iterators
            itMag.GoToBeginOfLine();
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
            itOutVector[0].Set(LimitResult(calculator->getResults()["T1"]));
            itOutVector[1].Set(LimitResult(calculator->getResults()["R2"]));
            itOutVector[2].Set(LimitResult(calculator->getResults()["A"]));
            itOutVector[3].Set(LimitResult(calculator->getResults()["B"]));
            itOutVector[4].Set(LimitResult(calculator->getResults()["T1star"]));
            itOutVector[5].Set(LimitResult(calculator->getResults()["ChiSqrt"]));
            itOutVector[6].Set(LimitResult(calculator->getResults()["SNR"]));
            itOutVector[7].Set(LimitResult(calculator->getResults()["NShmolliSamplesUsed"]));
            itOutVector[8].Set(LimitResult(calculator->getResults()["SD_A"]));
            itOutVector[9].Set(LimitResult(calculator->getResults()["SD_B"]));
            itOutVector[10].Set(LimitResult(calculator->getResults()["SD_T1"]));
            itOutVector[11].Set(LimitResult(calculator->getResults()["T2"]));

            if (calculator->getSigns()) {
                itOutMagSignRecov.GoToBeginOfLine();
                while (!itOutMagSignRecov.IsAtEndOfLine()) {
                    idx = itOutMagSignRecov.GetIndex();
                    PixelTypeOut temp = calculator->getSigMag()[idx[2]] * calculator->getSigns()[idx[2]] + 4095;
                    itOutMagSignRecov.Set(temp);
                    ++itOutMagSignRecov;
                }
            }

            // iterate all iterators accordingly
            for (unsigned int i = 0; i < outputImagesVector.size(); ++i) {
                ++itOutVector[i];
            }
            itMag.NextLine();
            if (imagePha) itPha.NextLine();
            itOutMagSignRecov.NextLine();
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

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetT1Image() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(0) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetR2Image() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(1) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetAImage() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(2) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetBImage() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(3) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetT1starImage() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(4) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetChiSqrtImage(){
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(5) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetSNRImage() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(6) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetNShmolliSamplesUsedImage() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(7) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetSD_AImage() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(8) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetSD_BImage() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(9) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetSD_T1Image() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(10) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut1*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetT2Image() {
        return dynamic_cast< TImageOut1 * >(
                this->ProcessObject::GetOutput(11) );
    }

    template< typename TImageIn, typename TImageOut1, typename TImageOut2>
    TImageOut2*
    CalculatorT1ImageFilter<TImageIn, TImageOut1, TImageOut2>
    ::GetMagSignRecovered(){
        return dynamic_cast< TImageOut2 * >(this->ProcessObject::GetOutput(12) );
    }

}// end namespace itk

#endif // USE_ITK

#endif //itkCalculatorT1ImageFilter_txx