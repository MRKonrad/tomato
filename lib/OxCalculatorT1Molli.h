/*!
 * \file OxCalculatorT1Molli.h
 * \author Konrad Werys
 * \date 2018/08/01
 */

#ifndef OXSHMOLLI2_OXCALCULATORT1MOLLI_H
#define OXSHMOLLI2_OXCALCULATORT1MOLLI_H

#include "OxCalculatorT1.h"

namespace Ox {

    /**
     * \class OxCalculatorT1Molli
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class CalculatorT1Molli : public CalculatorT1<MeasureType> {
    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculate(){

            // some initialisation
            const double MaxTIForSignInvert = (this->MAX_T1_TRESHOLD * 0.67);
            MeasureType lastValue = 1e32;
            MeasureType lastValueTemp = lastValue;
            MeasureType tempParameters[3];

            // get ready
            this->prepareToCalculate();

            // configure Functions object and fitter object
            this->getFunctionsT1()->setNSamples(this->getNSamples());
            this->getFunctionsT1()->setSignal(this->_Signal);
            this->getFunctionsT1()->setInvTimes(this->getInvTimes());
            this->getFunctionsT1()->setParameters(tempParameters);
            this->getFunctionsT1()->copyToParameters(this->_StartPoint); // start from the starting point

            this->getFitter()->setFunctionsT1(this->getFunctionsT1());

            // fit
            this->getFitter()->performFitting();

            // save the results at the best results
            KWUtil::copyArrayToArray(3, this->_Results, this->getFunctionsT1()->getParameters());
            lastValue = this->getFunctionsT1()->calcCostValue();

            // look for better solutions than the above one
            for (int iSwap = 0; iSwap < this->getNSamples(); iSwap++) {

                // continue only if TI in reasonable range
                if (this->_InvTimes[iSwap] > MaxTIForSignInvert) continue;

                // continue if sign was already calculated before
                if (this->_Signs[iSwap] != 0) continue;

                // in each iteration change the sign of one of the signal elements
                this->_Signal[iSwap] = -fabs(this->getSigMag()[iSwap]);

                // start from the starting point
                this->getFunctionsT1()->copyToParameters(this->_StartPoint);

                // fit
                this->getFitter()->performFitting();
                lastValueTemp = this->getFunctionsT1()->calcCostValue();

                // are these the best results?
                if (lastValueTemp < lastValue) {
                    // save the results at the best results
                    KWUtil::copyArrayToArray(3, this->_Results, this->getFunctionsT1()->getParameters());
                    lastValue = lastValueTemp;
                }
            }

            return 0; // EXIT_SUCCESS
        }
    };

} //namespace Ox
#endif //OXSHMOLLI2_OXCALCULATORT1MOLLI_H
