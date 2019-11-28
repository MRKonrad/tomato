/*!
 * \file OxCalculatorT2Truncation.hxx
 * \author Konrad Werys
 * \date 2019/11/25
 */

#ifndef Tomato_OXCalculatorT2Truncation_HXX
#define Tomato_OXCalculatorT2Truncation_HXX

#include "OxCalculatorT2Truncation.h"

namespace Ox {

    template< typename MeasureType >
    int
    CalculatorT2Truncation<MeasureType>
    ::calculate(){

        // some initialisation
        this->_Results = std::map <std::string, MeasureType>();

        // get ready, continue if prepareToCalculate EXIT_SUCCESS
        if (this->prepareToCalculate() == 1) {
            return 1; // EXIT_FAILURE
        }

        MeasureType tempParameters[3];
        KWUtil::copyArrayToArray(this->_nDims, tempParameters, this->_StartPoint); // start from the starting point

        this->getModel()->setNSamples(_NSamplesAboveSnrLimit);
        this->getModel()->setSignal(_SignalAboveSnrLimit);
        this->getModel()->setEchoTimes(_EchoTimesOfSignalAboveSnrLimit);

        // configure Fitter
        this->getFitter()->setParameters(tempParameters);
        this->getFitter()->setModel(this->getModel());

        // fit
        this->getFitter()->performFitting();

        MeasureType tempResults[3];
        KWUtil::copyArrayToArray(this->_nDims, tempResults, this->getFitter()->getParameters());
        if (tempResults[0] != 0) {
            if (this->_nDims == 2) {
                this->_Results["A"]  = tempResults[0];
                this->_Results["T2"] = tempResults[1];
                this->_Results["R2"] = calculateR2AbsFromModel(_NSamplesAboveSnrLimit, _EchoTimesOfSignalAboveSnrLimit, _SignalAboveSnrLimit, tempResults);
            } else if (this->_nDims == 3){
                this->_Results["A"]  = tempResults[0];
                this->_Results["B"]  = tempResults[1];
                this->_Results["T2"] = tempResults[2];
                this->_Results["R2"] = calculateR2AbsFromModel(_NSamplesAboveSnrLimit, _EchoTimesOfSignalAboveSnrLimit, _SignalAboveSnrLimit, tempResults);
            }
        }

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    MeasureType
    CalculatorT2Truncation<MeasureType>
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
    CalculatorT2Truncation<MeasureType>
    ::prepareToCalculate(){

        // if fitter does not have to iterate, do not calculate
        if (this->getFitter()->getMaxFunctionEvals() == 0){
            return 1; // EXIT_FAILURE
        }

        // verify echoTimes are sorted
        for (int i = 0; i < this->getNSamples()-1; i++){
            if (this->getEchoTimes()[i] > this->getEchoTimes()[i+1]){
                throw std::runtime_error("CalculatorT2Truncation: Echo times have to be sorted!");
            }
        }

        // calculate if higher than the cutoff
        if (KWUtil::calcMeanArray(this->getNSamples(), this->getSigMag()) < this->getMeanCutOff()) {
            return 0; // EXIT_SUCCESS
        }

        // calculate signal
        if(!this->getNoise()){
            throw std::runtime_error("CalculatorT2Truncation: I need noise values to perform calculations!");
        }
        delete [] _SignalAboveSnrLimit;
        delete [] _EchoTimesOfSignalAboveSnrLimit;
        _SignalAboveSnrLimit = new MeasureType[this->getNSamples()];
        _EchoTimesOfSignalAboveSnrLimit = new MeasureType[this->getNSamples()];
        _NSamplesAboveSnrLimit = 0;
        for (int i = 0; i < this->getNSamples()-1; i++){
            if (this->getSigMag()[i] / this->getNoise()[i] > _NoiseLimit){
                _SignalAboveSnrLimit[_NSamplesAboveSnrLimit] = this->getSigMag()[i];
                _EchoTimesOfSignalAboveSnrLimit[_NSamplesAboveSnrLimit] = this->getEchoTimes()[i];
                _NSamplesAboveSnrLimit ++;
            }
        }

        if (_NSamplesAboveSnrLimit < this->getNDims() + 1 ){
            return 1; // EXIT_FAILURE
        }

        // calculate start point
        MeasureType const defaultStartPoint2[] = {500, 100};
        MeasureType const defaultStartPoint3[] = {1, 500, 500};

        if (!this->getStartPointCalculator()){
            // no start point calculator
            if (this->_nDims == 2) {
                KWUtil::copyArrayToArray(this->_nDims, this->_StartPoint, defaultStartPoint2);
            } else if (this->_nDims == 3) {
                KWUtil::copyArrayToArray(this->_nDims, this->_StartPoint, defaultStartPoint3);
            } else {
                throw std::runtime_error("CalculatorT2Truncation: No default start point for this number of dimensions, "
                                         "consider using StartPointCalculator ");
            }
        } else {
            throw std::runtime_error("CalculatorT2Truncation: No implementation for StartPointCalculator");
        }

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    const MeasureType *
    CalculatorT2Truncation<MeasureType>
    ::getEchoTimes() const {
        if (!this->_EchoTimes) {
            throw std::runtime_error("_EchoTimes equals 0. Set _EchoTimes");
        };
        return this->_EchoTimes;
    }


} //namespace Ox

#endif //Tomato_OXCalculatorT2Truncation_HXX
