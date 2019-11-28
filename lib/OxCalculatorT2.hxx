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

        // some initialisation
        this->_Results = std::map <std::string, MeasureType>();

        // get ready, continue if prepareToCalculate EXIT_SUCCESS
        if (this->prepareToCalculate() == 1) {
            return 1; // EXIT_FAILURE
        }

        // calculate if higher than the cutoff
        if (KWUtil::calcMeanArray(this->getNSamples(), this->getSigMag()) < this->getMeanCutOff()) {
            return 0; // EXIT_SUCCESS
        }

        MeasureType tempParameters[3];
        KWUtil::copyArrayToArray(this->_nDims, tempParameters, this->_StartPoint); // start from the starting point

//        MeasureType tempSig[4];
//        KWUtil::copyArrayToArray(3, tempSig, this->getSigMag());
//        tempSig[3] = 0;
//
//        MeasureType tempEcho[4];
//        KWUtil::copyArrayToArray(3, tempEcho, this->getEchoTimes());
//        tempEcho[3] = 10000;
//
//        this->getModel()->setNSamples(4);
//        this->getModel()->setSignal(tempSig);
//        this->getModel()->setEchoTimes(tempEcho);

        this->getModel()->setNSamples(this->getNSamples());
        this->getModel()->setSignal(this->getSigMag());
        this->getModel()->setEchoTimes(this->getEchoTimes());

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
                this->_Results["R2"] = calculateR2AbsFromModel(this->getNSamples(), this->getEchoTimes(),this->getSigMag(), tempResults);
            } else if (this->_nDims == 3){
                this->_Results["A"]  = tempResults[0];
                this->_Results["B"]  = tempResults[1];
                this->_Results["T2"] = tempResults[2];
                this->_Results["R2"] = calculateR2AbsFromModel(this->getNSamples(), this->getEchoTimes(), this->getSigMag(), tempResults);
            }
        }

        return 0; // EXIT_SUCCESS
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
        MeasureType const defaultStartPoint2[] = {500, 100};
        MeasureType const defaultStartPoint3[] = {5, 100, 50};

        if (!this->getStartPointCalculator()){
            // no start point calculator
            if (this->_nDims == 2) {
                KWUtil::copyArrayToArray(this->_nDims, this->_StartPoint, defaultStartPoint2);
            } else if (this->_nDims == 3) {
                KWUtil::copyArrayToArray(this->_nDims, this->_StartPoint, defaultStartPoint3);
            } else {
                throw std::runtime_error("CalculatorT2: No default start point for this number of dimensions, "
                                         "consider using StartPointCalculator ");
            }
        } else {
            // start point calculator
            this->getStartPointCalculator()->setNDims(this->_nDims);
            if (!this->getStartPointCalculator()->getInputStartPoint()) {
                if (this->_nDims == 2) {
                    this->getStartPointCalculator()->setInputStartPoint(defaultStartPoint2);
                } else if (this->_nDims == 3) {
                    this->getStartPointCalculator()->setInputStartPoint(defaultStartPoint3);
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
        }

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
    const MeasureType *
    CalculatorT2<MeasureType>
    ::getEchoTimes() const {
        if (!this->_EchoTimes) {
            throw std::runtime_error("_EchoTimes equals 0. Set _EchoTimes");
        };
        return this->_EchoTimes;
    }

} //namespace Ox

#endif //Tomato_OXCALCULATORT2_HXX
