/*!
 * \file OxCalculatorT1Molli.hxx
 * \author Konrad Werys
 * \date 2019/01/15
 */

#ifndef Tomato_OXCALCULATORT1MOLLI_HXX
#define Tomato_OXCALCULATORT1MOLLI_HXX

namespace Ox {

    template< typename MeasureType >
    int
    CalculatorT1Molli<MeasureType>
    ::prepareToCalculate(){

        // if fitter does not have to iterate, do not calculate
        if (this->getFitter()->getMaxFunctionEvals() == 0){
            return 1; // EXIT_FAILURE
        }

        // check if ndims has been set
        if (this->_nDims != 2 && this->_nDims != 3){
            throw std::runtime_error("CalculatorT1Molli::prepareToCalculate currently implemented only for _nDims of 2 or 3");
        }

        // verify invTimes are sorted
        for (int i = 0; i < this->getNSamples()-1; i++){
            if (this->getInvTimes()[i] > this->getInvTimes()[i+1]){
                throw std::runtime_error("InvTimes have to be sorted!");
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
                this->_Signs[i] = 1; // no flip
            }
        }

        // calculate start point
        if (this->getSignCalculator()) {
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
                this->_StartPoint[i] = 1;
            }
        }

        return 0; // EXIT_SUCCESS
    };

    template< typename MeasureType >
    int
    CalculatorT1Molli<MeasureType>
    ::calculate(){

        // calculate if higher than the cutoff
        if (KWUtil::calcMeanArray(this->getNSamples(), this->getSigMag()) < this->getMeanCutOff()) {
            return 0; // EXIT_SUCCESS
        }

        // get ready, continue if prepareToCalculate EXIT_SUCCESS
        if (this->prepareToCalculate() == 1) {
            return 1; // EXIT_FAILURE
        }

        this->_Results = calculateMolli( this->getNSamples(),
                                         this->getInvTimes(),
                                         this->getSignal(),
                                         this->getSigns());

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    std::map <std::string, MeasureType>
    CalculatorT1Molli<MeasureType>
    ::calculateMolli(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs){

        // some initialisation
        std::map <std::string, MeasureType> results;

        MeasureType lastValue = 1e32;
        MeasureType lastValueTemp = 1e32;
        MeasureType *tempParameters = new MeasureType[this->_nDims];
        MeasureType *tempResults = new MeasureType[this->_nDims];

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
        KWUtil::copyArrayToArray(this->_nDims, tempResults, this->getFitter()->getParameters());
        lastValue = this->getModel()->calcCostValue(this->getFitter()->getParameters());

        // look for better solutions than the above one
        for (int iSwap = 0; iSwap < nSamples; iSwap++) {

            // continue only if TI in reasonable range
            if (invTimes[iSwap] > this->MaxTIForSignInvert) continue;

            // continue if sign was already calculated before
            if (signs[iSwap] != 0) continue;

            // in each iteration change the sign of one of the signal elements
            signal[iSwap] = -fabs(signal[iSwap]);

            // start from the starting point
            this->getFitter()->copyToParameters(this->_StartPoint);

            // fit
            this->getFitter()->performFitting();
            lastValueTemp = this->getModel()->calcCostValue(this->getFitter()->getParameters());

            // are these the best tempResults?
            if (lastValueTemp < lastValue) {
                // save the tempResults at the best tempResults
                KWUtil::copyArrayToArray(this->_nDims, tempResults, this->getFitter()->getParameters());
                lastValue = lastValueTemp;
            }
        }

        if (lastValue != 1e32 && tempResults[0] != 0) {
            if (this->_nDims == 2) {
                results["A"] = tempResults[0];
                results["T1"] = tempResults[1];
                results["R2"] = calculateR2AbsFromModel(nSamples, invTimes, signal, tempResults);
                results["ChiSqrt"] = KWUtil::getChiSqrt(lastValue, nSamples);
                results["LastValue"] = lastValue;
            } else if (this->_nDims == 3) {
                results["T1"] = tempResults[2] * (tempResults[1] / tempResults[0] - 1.);
                results["R2"] = calculateR2AbsFromModel(nSamples, invTimes, signal, tempResults);
                results["A"] = tempResults[0];
                results["B"] = tempResults[1];
                results["T1star"] = tempResults[2];
                results["ChiSqrt"] = KWUtil::getChiSqrt(lastValue, nSamples);
                results["SNR"] = (results["B"] - results["A"]) / (results["ChiSqrt"] + 0.001);
                results["LastValue"] = lastValue;

                MeasureType covarianceMatrix[3 * 3];
                calculateCovarianceMatrix(tempResults, covarianceMatrix);
                if (covarianceMatrix[4] > 0)
                    results["SD_A"] = sqrt(covarianceMatrix[4]); //  (1,1)
                if (covarianceMatrix[8] > 0)
                    results["SD_B"] = sqrt(covarianceMatrix[8]); //  (2,2)
                if (covarianceMatrix[0] > 0)
                    results["SD_T1"] = sqrt(covarianceMatrix[0]); // (0,0)
            }

        }

        delete [] tempParameters;
        delete [] tempResults;

        return results;
    }

    template< typename MeasureType >
    MeasureType
    CalculatorT1Molli<MeasureType>
    ::calculateR2AbsFromModel(int nSamples, const MeasureType* times, const MeasureType* signal, const MeasureType* parameters) {

        MeasureType *absFitted  = new MeasureType[nSamples];
        MeasureType *absYsignal = new MeasureType[nSamples];

        for (int i = 0; i < nSamples; i++){
            MeasureType fitted;
            fitted = this->_Model->calcModelValue(parameters, times[i]);
            absFitted[i] = fabs(fitted);
            absYsignal[i] = fabs(signal[i]);
        }

        double result = KWUtil::calcR2cor(nSamples, absFitted, absYsignal);

        delete[] absFitted;
        delete[] absYsignal;
        return result;
    }

    template< typename MeasureType >
    int
    CalculatorT1Molli<MeasureType>
    ::calculateCovarianceMatrix(const MeasureType* parameters, MeasureType *covarianceMatrix) {

        for (int i = 0; i < 3*3; ++i) {
            covarianceMatrix[i] = 0;
        }

        if (_DoCalculateSDMap) {
            int nSamples = this->getNSamples();
            const MeasureType *invTimes = this->getModel()->getInvTimes();

            MeasureType *residuals = new MeasureType[nSamples];
            MeasureType invCovarianceMatrix[3 * 3];

            this->getModel()->calcLSResiduals(parameters, residuals);

            calculateInvCovarianceMatrix(invTimes, residuals, parameters, invCovarianceMatrix);

            KWUtil::calcMatrixInverse3x3<MeasureType>(invCovarianceMatrix, covarianceMatrix);

            delete[] residuals;
        }

        return 0; //EXIT_SUCCESS

    }

    template< typename MeasureType >
    int
    CalculatorT1Molli<MeasureType>
    ::calculateInvCovarianceMatrix(const MeasureType* invTimes, const MeasureType* residuals, const MeasureType* parameters, MeasureType *invCovarianceMatrix) {

        // invCovarianceMatrix - indexing by column,row

        int nSamples = this->getNSamples();
        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T1star = parameters[2];
        MeasureType tempInvCovarianceMatrix[3*3];

        for (int i = 0; i < 3*3; ++i) {
            invCovarianceMatrix[i] = 0;
            tempInvCovarianceMatrix[i] = 0;
        }

        if (fabs(A) < std::numeric_limits<MeasureType>::min())
            return 1; // EXIT_FAILURE
        if (fabs(T1star) < std::numeric_limits<MeasureType>::min())
            return 1; // EXIT_FAILURE

        MeasureType T1 = (B/A-1)*T1star;

        if (fabs(T1) < std::numeric_limits<MeasureType>::min())
            return 1; // EXIT_FAILURE

        MeasureType dydA = 0;
        MeasureType dydB = 0;
        MeasureType dydT1 = 0;

        for (int i = 0; i < nSamples; ++i){
            MeasureType invTime = invTimes[i];
            MeasureType myexparg = ( -invTime * ( B/A - 1) / T1);

            if ((myexparg > std::numeric_limits<MeasureType>::max_exponent)
                || (myexparg < std::numeric_limits<MeasureType>::min_exponent))
                return 1; //EXIT_FAILURE

            MeasureType myexp = exp (myexparg);
            dydA  = 1 - B * myexp * invTime * B / ( T1 * A * A);
            dydB  = -myexp + B * myexp * invTime / ( T1 * A );
            dydT1 = -B * myexp * invTime * (B/A-1) / ( T1 * T1);

            tempInvCovarianceMatrix[0] += dydT1 * dydT1; // (0,0)
            tempInvCovarianceMatrix[1] += dydA  * dydT1; // (0,1)
            tempInvCovarianceMatrix[2] += dydB  * dydT1; // (0,2)
            tempInvCovarianceMatrix[3] += dydT1 * dydA;  // (1,0)
            tempInvCovarianceMatrix[4] += dydA  * dydA;  // (1,1)
            tempInvCovarianceMatrix[5] += dydB  * dydA;  // (1,2)
            tempInvCovarianceMatrix[6] += dydT1 * dydB;  // (2,0)
            tempInvCovarianceMatrix[7] += dydA  * dydB;  // (2,1)
            tempInvCovarianceMatrix[8] += dydB  * dydB;  // (2,2)
        }

        MeasureType SD = KWUtil::calcStandardDeviationArray<MeasureType>(nSamples, residuals);

        if (fabs(SD) < std::numeric_limits<MeasureType>::min())
            return 1; //EXIT_FAILURE

        for (int i = 0; i < 3*3; ++i) {
            tempInvCovarianceMatrix[i] = tempInvCovarianceMatrix[i] / (SD * SD);
        }

        for (int i = 0; i < 3*3; ++i) {
            invCovarianceMatrix[i] = tempInvCovarianceMatrix[i];
        }

        return 0; // EXIT_SUCCESS
    }

    template<typename MeasureType>
    bool CalculatorT1Molli<MeasureType>::getDoCalculateSDMap() const {
        return _DoCalculateSDMap;
    }

    template<typename MeasureType>
    void CalculatorT1Molli<MeasureType>::setDoCalculateSDMap(bool _DoCalculateSDMap) {
        CalculatorT1Molli::_DoCalculateSDMap = _DoCalculateSDMap;
    }


} //namespace Ox

#endif //Tomato_OXCALCULATORT1MOLLI_HXX
