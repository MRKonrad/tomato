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

            //const int MAX_T1_TRESHOLD = 4000;
            //double MaxTIForSignInvert = (MAX_T1_TRESHOLD * 0.67);

            this->prepareToCalculate();

            // configure
            MeasureType lastValue = 1e32;
            MeasureType lastValueTemp = lastValue;

            // start from the starting point
            KWUtil::copyArrayToArray(3, this->_Results, this->_StartPoint);

            this->getNSamples();
            this->getFunctionsT1()->setNSamples(this->getNSamples());
            this->getFunctionsT1()->setSignal(this->_Signal);
            this->getFunctionsT1()->setInvTimes(this->getInvTimes());
            this->getFunctionsT1()->setParameters(this->_Results);

            this->getFitter()->setFunctionsT1(this->getFunctionsT1());

            // fit
            this->getFitter()->performFitting();
            lastValue = this->getFunctionsT1()->calcCostValue();

            for (int iswap = 0; iswap < this->getNSamples(); iswap++) {

                //if (this->_InvTimes[iswap] > MaxTIForSignInvert) continue; // continue only if sign is undefined (0)
                // continue if sign was already calculated before
                if (this->_Signs[iswap] != 0) continue;

                // change the sign of the signal
                this->_Signal[iswap] = -fabs(this->getSigMag()[iswap]);

                // start from the starting point
                MeasureType currentResults[3] = {0, 0, 0};
                KWUtil::copyArrayToArray(3, currentResults, this->_StartPoint);
                this->getFunctionsT1()->setParameters(currentResults);

                // fit
                this->getFitter()->performFitting();

                lastValueTemp = this->getFunctionsT1()->calcCostValue();

                if (lastValueTemp < lastValue) {
                    lastValue = lastValueTemp;
                    KWUtil::copyArrayToArray(3, this->_Results, currentResults);
                }
            }

            return 0; // EXIT_SUCCESS
        }
    };

} //namespace Ox
#endif //OXSHMOLLI2_OXCALCULATORT1MOLLI_H
