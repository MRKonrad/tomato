/*!
* \file OxCalculatorT1Shmolli.hxx
* \author Konrad Werys
* \date 2019/01/17
*/

#ifndef TOMATO_OXCALCULATORT1SHMOLLI_HXX
#define TOMATO_OXCALCULATORT1SHMOLLI_HXX

namespace Ox {

    template< typename MeasureType >
    int
    CalculatorT1Shmolli<MeasureType>
    ::prepareToCalculate(){

        // Shmolli 51111 needs exactly 7 samples
        if (this->getNSamples() != 7){
            throw std::runtime_error("T1_SHMOLLI reconstruction available only for 7 samples.");
        }

//            // base class prepareToCalculate()
//            if(CalculatorT1<MeasureType>::prepareToCalculate() == 1) {
//                return 1; // EXIT_FAILURE
//            }

        // if fitter does not have to iterate, do not calculate
        if (this->getFitter()->getMaxFunctionEvals() == 0){
            return 1; // EXIT_FAILURE
        }

        // verify invTimes are sorted
        for (int i = 0; i < this->getNSamples()-1; i++){
            if (this->getInvTimes()[i] > this->getInvTimes()[i+1]){
                throw std::runtime_error("InvTimes have to be sorted!");
            }
        }

        // calculate sign
        this->getSignCalculator()->setNSamples(this->getNSamples());
        this->getSignCalculator()->setInvTimes(this->getInvTimes());
        this->getSignCalculator()->setSigMag(this->getSigMag());
        this->getSignCalculator()->setSigPha(this->getSigPha());
        this->getSignCalculator()->setSignal(this->_Signal);
        this->getSignCalculator()->setSigns(this->_Signs);

        this->getSignCalculator()->calculateSign();

        // calculate TRR
        calculateTRRaverageHB();

        return 0; // EXIT_SUCCESS
    };

    template< typename MeasureType >
    int
    CalculatorT1Shmolli<MeasureType>
    ::getStartPointSKPShmolli(const MeasureType * invTimes5, const MeasureType * invTimes6, const MeasureType * invTimes7,
                              const MeasureType * sigMag5,   const MeasureType * sigMag6,   const MeasureType * sigMag7,
                              const MeasureType * signs5,    const MeasureType * signs6,    const MeasureType * signs7){

        int nStartFitPoints = 0;

        StartPointCalculator<MeasureType> *startPointCalculator = this->getStartPointCalculator();
        startPointCalculator->setCalculatedStartPoint(this->_StartPoint);

        if (this->getSigPha()){
            startPointCalculator->setNSamples(5);
            startPointCalculator->setInvTimes(invTimes5);
            startPointCalculator->setSigMag(sigMag5);
            startPointCalculator->setSigns(signs5);
            nStartFitPoints = startPointCalculator->calculateStartPoint();

            if( nStartFitPoints < 3 ) {
                startPointCalculator->setNSamples(6);
                startPointCalculator->setInvTimes(invTimes6);
                startPointCalculator->setSigMag(sigMag6);
                startPointCalculator->setSigns(signs6);
                nStartFitPoints = startPointCalculator->calculateStartPoint();
            }
            if( nStartFitPoints < 3 ) {
                startPointCalculator->setNSamples(7);
                startPointCalculator->setInvTimes(invTimes7);
                startPointCalculator->setSigMag(sigMag7);
                startPointCalculator->setSigns(signs7);
                startPointCalculator->calculateStartPoint();
            }
        } else {
            if( nStartFitPoints < 3 ) {
                startPointCalculator->setNSamples(7);
                startPointCalculator->setInvTimes(invTimes7);
                startPointCalculator->setSigMag(sigMag7);
                startPointCalculator->setSigns(signs7);
                startPointCalculator->calculateStartPoint();
            }
        }

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    int
    CalculatorT1Shmolli<MeasureType>
    ::calculateTRRaverageHB(){

        // if shmolli (7 samples)
        const MeasureType *invTimes = this->getInvTimes();
        MeasureType invTimesDifferences[4];
        if (this->_nSamples == 7) {
            invTimesDifferences[0] = invTimes[3] - invTimes[0];
            invTimesDifferences[1] = invTimes[4] - invTimes[3];
            invTimesDifferences[2] = invTimes[5] - invTimes[4];
            invTimesDifferences[3] = invTimes[6] - invTimes[5];
        }

        _TRRaverageHB = KWUtil::calcMeanArray(4, invTimesDifferences);
        // m_TRRaverageHB = KWUtil::calcMedianArray(4, invTimesDifferences);

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    int
    CalculatorT1Shmolli<MeasureType>
    ::calculate(){

        this->_Results = CalculatorT1Results<MeasureType>();

        // calculate if higher than the cutoff
        if (KWUtil::calcMeanArray(this->getNSamples(), this->getSigMag()) < this->getMeanCutOff()) {
            return 0; // EXIT_SUCCESS
        }

        // continue only if prepareToCalculate was successful
        if(this->prepareToCalculate() == 1){ //EXIT_FAILURE
            return 1; // EXIT_FAILURE
        }

        // some initialisation
        const double MaxTIForSignInvert = (this->MAX_T1_TRESHOLD * 0.67);
        MeasureType lastValue = 1e32;
        MeasureType lastValueTemp = lastValue;

        // nShmolliSamplesUsed = 5 // =(T1s567[0] < TRRaverageHB)  &&  (T1s567[0] > 1);
        // nShmolliSamplesUsed = 6 // =(chi567[1]*T1s567[1] < (chi567[0]*TRRaverageHB )) && (T1s567[1] > 1);
        // nShmolliSamplesUsed = 7 // =((chi567[2]*T1s567[2]) < (chi567[1]* TRRaverageHB*0.4 )) && (T1s567[2] > 1);

        // temporary defs here
        double T1temp = 0;
        double chiTemp = 1e32; // legacy reasons
        unsigned int nShmolliSamplesUsed = 0;
        MeasureType TRRaverageHB = this->_TRRaverageHB;

        CalculatorT1Results<MeasureType> results0, results5, results6, results7;
        MeasureType signal5[5],   signal6[6],   signal7[7];
        MeasureType invTimes5[5], invTimes6[6], invTimes7[7];
        MeasureType signs5[5],    signs6[6],    signs7[7];

        getShmolliSamples(this->_InvTimes, invTimes5, invTimes6, invTimes7);
        getShmolliSamples(this->_Signal, signal5, signal6, signal7);
        getShmolliSamples(this->_Signs, signs5, signs6, signs7);

        this->getStartPointSKPShmolli(
                invTimes5, invTimes6, invTimes7,
                signal5, signal6, signal7,
                signs5, signs6, signs7);

        signs5[0] = -1; // KW: Why only in the 5 samples case and not in the 6 or 7 samples case? small amount of pixels (94) influenced by it
        signal5[0] = -fabs(signal5[0]);

        // 5els fitting
        results5 = this->calculateMolli(5, invTimes5, signal5, signs5);

        if (results5.A > 1){ // converged
            nShmolliSamplesUsed = 5;
            T1temp = results5.T1;
            chiTemp = results5.ChiSqrt; // legacy
        } else {
            chiTemp = results5.LastValue; // legacy, very small amount of pixels (3) influenced by it
        }

        // 6els fitting
        results6 = this->calculateMolli(6, invTimes6, signal6, signs6);

        if ((T1temp <= TRRaverageHB) // KW: has to be T1temp in these cases that do not converge
            || (results6.A <= 1) // not converged
            || (results6.T1 <= 0.4 * TRRaverageHB) // prevent from errors in v.short range. KW: very small amount of pixels (14) influenced by it
                )
        {

            if ((results6.A > 1) // converged
                && (results6.ChiSqrt * results6.T1 < chiTemp * TRRaverageHB))
            {
                nShmolliSamplesUsed = 6;
                T1temp = results6.T1;
            }

            // 7els fitting
            results7 = this->calculateMolli(7, invTimes7, signal7, signs7);

            if ((results7.A > 1) // converged
                && (results7.ChiSqrt * results7.T1 < results6.ChiSqrt * TRRaverageHB * 0.4) // KW: in the article results5 instead of 6. 1422 pixels influenced by change
                && (T1temp < TRRaverageHB) // KW: not a single pixel influenced by it
                    )
            {
                nShmolliSamplesUsed = 7; // use all 3 IR only if SD improvement compensates for a limit of 5*MAxT1<2*T_HB
            }
        }

        // assign output values
        if      (nShmolliSamplesUsed == 5) {
            this->_Results = results5;
        }
        else if (nShmolliSamplesUsed == 6) {
            this->_Results = results6;
        }
        else if (nShmolliSamplesUsed == 7) {
            this->_Results = results7;
        } else {
            this->_Results = results0;
            //std::cout << "ShMOLLI calculation error" << std::endl;
            //throw itk::ExceptionObject(__FILE__, __LINE__, "ShMOLLI calculation error");
        }
        this->_Results.NShmolliSamplesUsed = nShmolliSamplesUsed;
        this->_Results.hasBeenCalculated = true;

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    int
    CalculatorT1Shmolli<MeasureType>
    ::getShmolliSamples(const MeasureType* inArray, MeasureType* outArray5, MeasureType* outArray6, MeasureType* outArray7){
        int indexes7[] = {0,1,2,3,4,5,6};
        int indexes6[] = {0,1,3,4,5,6};
        int indexes5[] = {0,3,4,5,6};
        for (int i = 0; i < 7; i++) outArray7[i] = inArray[indexes7[i]];
        for (int i = 0; i < 6; i++) outArray6[i] = inArray[indexes6[i]];
        for (int i = 0; i < 5; i++) outArray5[i] = inArray[indexes5[i]];
        return 0; // EXIT_SUCCESS
    }

} //namespace Ox

#endif //TOMATO_OXCALCULATORT1SHMOLLI_HXX
