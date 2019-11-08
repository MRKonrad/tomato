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
    ::calculate(){

        this->_Results = CalculatorT1Results<MeasureType>();

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
    CalculatorT1Results<MeasureType>
    CalculatorT1Molli<MeasureType>
    ::calculateMolli(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs){

        // some initialisation
        CalculatorT1Results<MeasureType> resultsStruc;

        MeasureType lastValue = 1e32;
        MeasureType lastValueTemp = 1e32;
        MeasureType tempParameters[3];
        MeasureType tempResults[3];

        KWUtil::copyArrayToArray(3, tempParameters, this->_StartPoint); // start from the starting point

        // configure Functions object and fitter object
        this->getModelT1()->setNSamples(nSamples);
        this->getModelT1()->setSignal(signal);
        this->getModelT1()->setInvTimes(invTimes);

        // configure Fitter
        this->getFitter()->setParameters(tempParameters);
        this->getFitter()->setModelT1(this->getModelT1());

        // fit
        this->getFitter()->performFitting();

        // save the tempResults at the best tempResults
        KWUtil::copyArrayToArray(3, tempResults, this->getFitter()->getParameters());
        lastValue = this->getModelT1()->calcCostValue(this->getFitter()->getParameters());

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
            lastValueTemp = this->getModelT1()->calcCostValue(this->getFitter()->getParameters());

            // are these the best tempResults?
            if (lastValueTemp < lastValue) {
                // save the tempResults at the best tempResults
                KWUtil::copyArrayToArray(3, tempResults, this->getFitter()->getParameters());
                lastValue = lastValueTemp;
            }
        }

        if (lastValue != 1e32 && tempResults[0] != 0) {
            resultsStruc.T1     = tempResults[2] * (tempResults[1] / tempResults[0] - 1.);
            resultsStruc.R2     = calculateR2AbsFromModel(nSamples, invTimes, signal, tempResults);
            resultsStruc.A      = tempResults[0];
            resultsStruc.B      = tempResults[1];
            resultsStruc.T1star = tempResults[2];
            resultsStruc.ChiSqrt = KWUtil::getChiSqrt(lastValue, nSamples);
            resultsStruc.SNR =  (resultsStruc.B - resultsStruc.A) / (resultsStruc.ChiSqrt + 0.001);
            resultsStruc.LastValue = lastValue;

            MeasureType covarianceMatrix[3*3];
            calculateCovarianceMatrix(tempResults, covarianceMatrix);
            if (covarianceMatrix[4] > 0)
                resultsStruc.SD_A = sqrt(covarianceMatrix[4]); //  (1,1)
            if (covarianceMatrix[8] > 0)
                resultsStruc.SD_B = sqrt(covarianceMatrix[8]); //  (2,2)
            if (covarianceMatrix[0] > 0)
                resultsStruc.SD_T1 = sqrt(covarianceMatrix[0]); // (0,0)

        }

        return resultsStruc;
    }

    template< typename MeasureType >
    int
    CalculatorT1Molli<MeasureType>
    ::prepareToCalculate(){

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

        // calculate start point
        this->getStartPointCalculator()->setNDims(this->_nDims);
        if (!this->getStartPointCalculator()->getInputStartPoint()){
            if (this->_nDims == 2){
                MeasureType const temp[] = {100, 1000};
                this->getStartPointCalculator()->setInputStartPoint(temp);
            } else if (this->_nDims == 3){
                MeasureType const temp[] = {100, 200, 1000};
                this->getStartPointCalculator()->setInputStartPoint(temp);
            } else {
                throw std::runtime_error("Calculator: Set InputStartPoint in StartPointCalculator");
            }
        }
        this->getStartPointCalculator()->setNSamples(this->getNSamples());
        this->getStartPointCalculator()->setInvTimes(this->getInvTimes());
        this->getStartPointCalculator()->setSigMag(this->getSigMag());
        this->getStartPointCalculator()->setSigns(this->getSigns());
        this->getStartPointCalculator()->setCalculatedStartPoint(this->_StartPoint);

        this->getStartPointCalculator()->calculateStartPoint();

        return 0; // EXIT_SUCCESS
    };

    template< typename MeasureType >
    MeasureType
    CalculatorT1Molli<MeasureType>
    ::calculateR2AbsFromModel(int nSamples, const MeasureType* invTimes, const MeasureType* signal, const MeasureType* parameters) {

        //int nSamples = this->getNSamples();
        MeasureType *absFitted  = new MeasureType[nSamples];
        MeasureType *absYsignal = new MeasureType[nSamples];

        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T1star = parameters[2];

        for (int i = 0; i < nSamples; i++){
            MeasureType fitted;
            fitted = A - B * exp(-invTimes[i] / T1star);
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
            const MeasureType *invTimes = this->getModelT1()->getInvTimes();

            MeasureType *residuals = new MeasureType[nSamples];
            MeasureType invCovarianceMatrix[3 * 3];

            //this->getModelT1()->copyToParameters(parameters);
            this->getModelT1()->calcLSResiduals(parameters, residuals);

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
