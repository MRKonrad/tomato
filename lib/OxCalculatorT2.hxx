/*!
 * \file OxCalculatorT2.hxx
 * \author Konrad Werys
 * \date 2019/11/05
 */

#ifndef Tomato_OXCALCULATORT2_HXX
#define Tomato_OXCALCULATORT2_HXX

namespace Ox {

    template< typename MeasureType >
    int
    CalculatorT2<MeasureType>
    ::calculate(){

        // get ready, continue if prepareToCalculate EXIT_SUCCESS
        if (this->prepareToCalculate() == 1) {
            return 1; // EXIT_FAILURE
        }

        CalculatorT1Results<MeasureType> resultsStruc;

        MeasureType tempParameters[3];
        KWUtil::copyArrayToArray(3, tempParameters, this->_StartPoint); // start from the starting point

        this->getModelT1()->setNSamples(this->getNSamples());
        this->getModelT1()->setSignal(this->getSigMag());
        this->getModelT1()->setEchoTimes(this->getEchoTimes());

        // configure Fitter
        this->getFitter()->setParameters(tempParameters);
        this->getFitter()->setModelT1(this->getModelT1());

        // fit
        this->getFitter()->performFitting();

        MeasureType tempResults[3];
        KWUtil::copyArrayToArray(3, tempResults, this->getFitter()->getParameters());
        if (tempResults[0] != 0) {
            resultsStruc.A      = tempResults[0];
            resultsStruc.B      = tempResults[1];
            resultsStruc.T1     = tempResults[2];
            resultsStruc.T2     = tempResults[2];
            resultsStruc.R2     = calculateR2AbsFromModel(this->getNSamples(), this->getEchoTimes(), this->getSigMag(), tempResults);
        }

        this->_Results = resultsStruc;

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    MeasureType
    CalculatorT2<MeasureType>
    ::calculateR2AbsFromModel(int nSamples, const MeasureType* times, const MeasureType* signal, const MeasureType* parameters) {

        MeasureType *absFitted  = new MeasureType[nSamples];
        MeasureType *absYsignal = new MeasureType[nSamples];

        for (int i = 0; i < nSamples; i++){
            MeasureType fitted;
            fitted = this->_ModelT1->calcModelValue(parameters, times[i]);
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
    CalculatorT2<MeasureType>
    ::prepareToCalculate(){

        // if fitter does not have to iterate, do not calculate
        if (this->getFitter()->getMaxFunctionEvals() == 0){
            return 1; // EXIT_FAILURE
        }

        // verify echoTimes are sorted
        for (int i = 0; i < this->getNSamples()-1; i++){
            if (this->getEchoTimes()[i] > this->getEchoTimes()[i+1]){
                throw std::runtime_error("Echo times have to be sorted!");
            }
        }

        // calculate start point
        this->getStartPointCalculator()->setNDims(this->_nDims);
        if (!this->getStartPointCalculator()->getInputStartPoint()){
            if (this->_nDims == 2){
                MeasureType const temp[] = {100, 50};
                this->getStartPointCalculator()->setInputStartPoint(temp);
            } else if (this->_nDims == 3){
                MeasureType const temp[] = {1, 100, 50};
                this->getStartPointCalculator()->setInputStartPoint(temp);
            } else {
                throw std::runtime_error("CalculatorT2: Set InputStartPoint in StartPointCalculator");
            }
        }
        this->getStartPointCalculator()->setNSamples(this->getNSamples());
        this->getStartPointCalculator()->setEchoTimes(this->getEchoTimes());
        this->getStartPointCalculator()->setSigMag(this->getSigMag());
        this->getStartPointCalculator()->setSigns(this->getSigns());
        this->getStartPointCalculator()->setCalculatedStartPoint(this->_StartPoint);

        this->getStartPointCalculator()->calculateStartPoint();

        return 0; // EXIT_SUCCESS
    };

    template<typename MeasureType>
    bool CalculatorT2<MeasureType>::getDoCalculateSDMap() const {
        return _DoCalculateSDMap;
    }

    template<typename MeasureType>
    void CalculatorT2<MeasureType>::setDoCalculateSDMap(bool _DoCalculateSDMap) {
        CalculatorT2::_DoCalculateSDMap = _DoCalculateSDMap;
    }


} //namespace Ox

#endif //Tomato_OXCALCULATORT2_HXX
