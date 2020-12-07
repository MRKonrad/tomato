/*!
 * \file OxCalculatorT1WithSignCheck.hxx
 * \author Konrad Werys
 * \date 2019/01/15
 */

#ifndef Tomato_OXCALCULATORT1WithSignCheck_HXX
#define Tomato_OXCALCULATORT1WithSignCheck_HXX

namespace Ox {

    template< typename MeasureType >
    int
    CalculatorT1WithSignCheck<MeasureType>
    ::prepareToCalculate(){

        // if fitter does not have to iterate, do not calculate
        if (this->getFitter()->getMaxFunctionEvals() == 0){
            return 1; // EXIT_FAILURE
        }

        // check if ndims has been set
        if (this->_nDims != 2 && this->_nDims != 3){
            throw std::runtime_error("CalculatorT1WithSignCheck::prepareToCalculate currently implemented only for _nDims of 2 or 3");
        }

        if (!this->getInvTimes()){
            throw std::runtime_error("CalculatorT1WithSignCheck::prepareToCalculate InvTimes have to be provided!");
        }

        // verify invTimes are sorted
        for (int i = 0; i < this->getNSamples()-1; i++){
            if (this->getInvTimes()[i] > this->getInvTimes()[i+1]){
                throw std::runtime_error("CalculatorT1WithSignCheck::prepareToCalculate InvTimes have to be sorted!");
            }
        }

        // calculate sign
        if (this->getSignCalculator()) {

            this->getSignCalculator()->setNSamples(this->getNSamples());
            this->getSignCalculator()->setInvTimes(this->getInvTimes());
            this->getSignCalculator()->setSigMag(this->getSigMag());
            this->getSignCalculator()->setSigPha(this->getSigPha());
            this->getSignCalculator()->setSignal(this->_Signal);
            this->getSignCalculator()->setSigns(this->_Signs);

            this->getSignCalculator()->calculateSign();

        } else {
            for (int i = 0; i < this->_nSamples; ++i){
                this->_Signal[i] = this->_SigMag[i];
                this->_Signs[i] = 1; // no flip
            }
        }

        // calculate start point
        if (this->getStartPointCalculator()) {
            this->getStartPointCalculator()->setNDims(this->_nDims);
            if (!this->getStartPointCalculator()->getInputStartPoint()) {
                if (this->_nDims == 2) {
                    MeasureType const temp[] = {100, 1000};
                    this->getStartPointCalculator()->setInputStartPoint(temp);
                } else if (this->_nDims == 3) {
                    MeasureType const temp[] = {100, 200, 1000};
                    this->getStartPointCalculator()->setInputStartPoint(temp);
                }
            }
            this->getStartPointCalculator()->setNSamples(this->getNSamples());
            this->getStartPointCalculator()->setInvTimes(this->getInvTimes());
            this->getStartPointCalculator()->setSigMag(this->getSigMag());
            this->getStartPointCalculator()->setSigns(this->getSigns());
            this->getStartPointCalculator()->setCalculatedStartPoint(this->_StartPoint);

            this->getStartPointCalculator()->calculateStartPoint();
        } else {
            for (int i = 0; i < this->_nDims; ++i){
                this->_StartPoint[i] = 500;
            }
        }

        return 0; // EXIT_SUCCESS
    };

    template< typename MeasureType >
    int
    CalculatorT1WithSignCheck<MeasureType>
    ::calculate(){

        this->_Results = std::map <std::string, MeasureType>();

        // calculate if higher than the cutoff
        if (KWUtil::calcMeanArray(this->getNSamples(), this->getSigMag()) < this->getMeanCutOff()) {
            return 0; // EXIT_SUCCESS
        }

        // get ready, continue if prepareToCalculate EXIT_SUCCESS
        if (this->prepareToCalculate() == 1) {
            return 1; // EXIT_FAILURE
        }

        this->_Results = calculateWithSignCheck( this->getNSamples(),
                                         this->getInvTimes(),
                                         this->getSignal(),
                                         this->getSigns());

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    std::map <std::string, MeasureType>
    CalculatorT1WithSignCheck<MeasureType>
    ::calculateWithSignCheck(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs){

        // some initialisation
        std::map <std::string, MeasureType> results;

        MeasureType mse = 1e32;
        MeasureType mseTemp = 1e32;
        MeasureType *tempParameters = new MeasureType[this->_nDims];
        MeasureType *calculatedParameters = new MeasureType[this->_nDims];
        int timeFlip = 0;

        KWUtil::copyArrayToArray(this->_nDims, tempParameters, this->_StartPoint); // start from the starting point

        // configure Functions object and fitter object
        this->getModel()->setNSamples(nSamples);
        this->getModel()->setSignal(signal);
        this->getModel()->setInvTimes(invTimes);

        // configure Fitter
        this->getFitter()->setParameters(tempParameters);
        this->getFitter()->setModel(this->getModel());

        // fit
        this->getFitter()->performFitting();

        // save the tempResults at the best tempResults
        KWUtil::copyArrayToArray(this->_nDims, calculatedParameters, this->getFitter()->getParameters());
        mse = this->getFitter()->getMse();

        // look for better solutions than the above one
        for (int iSwap = 0; iSwap < nSamples; iSwap++) {

            // continue if sign was already calculated before
            if (signs[iSwap] != 0) continue;
            signs[iSwap] = 1;

            // continue only if TI in reasonable range
            if (invTimes[iSwap] > this->MaxTIForSignInvert) continue;

            // in each iteration change the sign of one of the signal elements
            signal[iSwap] = -fabs(signal[iSwap]);

            // start from the starting point
            this->getFitter()->copyToParameters(this->_StartPoint);

            // fit
            this->getFitter()->performFitting();
            mseTemp = this->getFitter()->getMse();

            // are these the best tempResults?
            if (mseTemp < mse) {
                // save the tempResults at the best tempResults
                KWUtil::copyArrayToArray(this->_nDims, calculatedParameters, this->getFitter()->getParameters());
                mse = mseTemp;
                timeFlip = iSwap;
            }
        }

        // sign and signal for the best fit
        for (int i = 0; i < nSamples; i++) signs[i] = 1;
        for (int i = 0; i < timeFlip + 1; i++) signs[i] = -1;
        for (int i = 0; i < nSamples; i++) signal[i] = signs[i] * fabs(signal[i]);

        // errors
        int deltasCalculatedExit = 1;
        MeasureType *fitDeltas = new MeasureType[this->_nDims];
        MeasureType *jacobian = new MeasureType[nSamples*this->_nDims];
        this->getModel()->calcLSJacobian(calculatedParameters, jacobian);
        deltasCalculatedExit = KWUtil::calculateFitError(
                nSamples,
                this->_nDims,
                jacobian,
                sqrt(mse*nSamples),
                fitDeltas);
        delete [] jacobian;

        if (mse != 1e32 && calculatedParameters[0] != 0) {
            KWUtil::copyArrayToArray(this->_nDims, this->_ParametersAfterFitting, calculatedParameters);
            if (this->_nDims == 2) {
                results["A"] = calculatedParameters[0];
                results["T1star"] = calculatedParameters[1];
                results["R2"] = calculateR2FromModel(nSamples, invTimes, signal, calculatedParameters);
                results["R2Abs"] = calculateR2AbsFromModel(nSamples, invTimes, signal, calculatedParameters);
                results["ChiSqrt"] = KWUtil::getChiSqrt(mse*nSamples, nSamples);
                results["LastValue"] = mse*nSamples;
                results["timeFlip"] = timeFlip;

                results["deltaA"] = -1;
                results["deltaT1"] = -1;
                if (deltasCalculatedExit == 0) {
                    results["deltaA"] = fitDeltas[0];
                    results["deltaT1star"] = fitDeltas[1];
                }
            } else if (this->_nDims == 3) {
                results["T1"] = calculatedParameters[2] * (calculatedParameters[1] / calculatedParameters[0] - 1.);
                results["R2"] = calculateR2FromModel(nSamples, invTimes, signal, calculatedParameters);
                results["R2Abs"] = calculateR2AbsFromModel(nSamples, invTimes, signal, calculatedParameters);
                results["A"] = calculatedParameters[0];
                results["B"] = calculatedParameters[1];
                results["T1star"] = calculatedParameters[2];
                results["ChiSqrt"] = KWUtil::getChiSqrt(mse*nSamples, nSamples);
                results["SNR"] = (results["B"] - results["A"]) / (results["ChiSqrt"] + 0.001);
                results["LastValue"] = mse*nSamples;
                results["timeFlip"] = timeFlip;

                results["deltaA"] = -1;
                results["deltaB"] = -1;
                results["deltaT1star"] = -1;
                results["deltaT1"] = -1;
                if (deltasCalculatedExit == 0){
                    results["deltaA"] = fitDeltas[0];
                    results["deltaB"] = fitDeltas[1];
                    results["deltaT1star"] = fitDeltas[2];
                    results["deltaT1"] =
                            results["deltaT1star"] * (results["B"]/results["A"] - 1)
                            + results["T1star"] * ( results["deltaB"]/results["A"]
                            + results["B"]*results["deltaA"]/(results["A"] * results["A"]) );
                }
            }
        }

        delete [] tempParameters;
        delete [] calculatedParameters;
        delete [] fitDeltas;
        return results;
    }

    template< typename MeasureType >
    MeasureType
    CalculatorT1WithSignCheck<MeasureType>
    ::calculateR2FromModel(int nSamples, const MeasureType* times, const MeasureType* signal, const MeasureType* parameters) {

        MeasureType *fitted  = new MeasureType[nSamples];

        for (int i = 0; i < nSamples; i++){
            fitted[i] = this->_Model->calcModelValue(parameters, times[i]);
        }

        double result = KWUtil::calcR2cor(nSamples, fitted, signal);

        delete [] fitted;
        return result;
    }

    template< typename MeasureType >
    MeasureType
    CalculatorT1WithSignCheck<MeasureType>
    ::calculateR2AbsFromModel(int nSamples, const MeasureType* invTimes, const MeasureType* signal, const MeasureType* parameters) {


        MeasureType *absFitted  = new MeasureType[nSamples];
        MeasureType *absYsignal = new MeasureType[nSamples];

        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T1star = parameters[2];

        for (int i = 0; i < nSamples; i++){
            MeasureType fitted;
            fitted = this->_Model->calcModelValue(parameters, invTimes[i]);
            absFitted[i] = fabs(fitted);
            absYsignal[i] = fabs(signal[i]);
        }

        double result = KWUtil::calcR2cor(nSamples, absFitted, absYsignal);

        delete[] absFitted;
        delete[] absYsignal;
        return result;
    }

    template< typename MeasureType >
    const MeasureType *
    CalculatorT1WithSignCheck<MeasureType>
    ::getInvTimes() const {
        if (!this->_InvTimes) {
            throw std::runtime_error("_InvTimes equals 0. Set _InvTimes");
        };
        return this->_InvTimes;
    }

    template<typename MeasureType>
    bool CalculatorT1WithSignCheck<MeasureType>::getDoCalculateSDMap() const {
        return _DoCalculateSDMap;
    }

    template<typename MeasureType>
    void CalculatorT1WithSignCheck<MeasureType>::setDoCalculateSDMap(bool _DoCalculateSDMap) {
        CalculatorT1WithSignCheck::_DoCalculateSDMap = _DoCalculateSDMap;
    }


} //namespace Ox

#endif //Tomato_OXCALCULATORT1WithSignCheck_HXX
