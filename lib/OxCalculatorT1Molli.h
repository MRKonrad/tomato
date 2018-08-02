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

        CalculatorT1Molli() : CalculatorT1<MeasureType>(){
            MaxTIForSignInvert = this->MAX_T1_TRESHOLD * 0.67;
        }

        virtual int calculate(){
            MeasureType results[3];
            this->_Results = calculateMolli(this->getNSamples(),
                                             this->getInvTimes(),
                                             this->getSignal(),
                                             this->getSigns());

            return 0; // EXIT_SUCCESS
        }

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual CalculatorT1Results<MeasureType> calculateMolli(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs){

            // some initialisation
            MeasureType lastValue = 1e32;
            MeasureType lastValueTemp = 1e32;
            MeasureType tempParameters[3];
            MeasureType tempResults[3];
            CalculatorT1Results<MeasureType> restulsStruct;

            // get ready
            this->prepareToCalculate();

            // configure Functions object and fitter object
            this->getFunctionsT1()->setNSamples(nSamples);
            this->getFunctionsT1()->setSignal(signal);
            this->getFunctionsT1()->setInvTimes(invTimes);
            this->getFunctionsT1()->setParameters(tempParameters);
            this->getFunctionsT1()->copyToParameters(this->_StartPoint); // start from the starting point

            this->getFitter()->setFunctionsT1(this->getFunctionsT1());

            // fit
            this->getFitter()->performFitting();

            // save the tempResults at the best tempResults
            KWUtil::copyArrayToArray(3, tempResults, this->getFunctionsT1()->getParameters());
            lastValue = this->getFunctionsT1()->calcCostValue();

            // look for better solutions than the above one
            for (int iSwap = 0; iSwap < nSamples; iSwap++) {

                // continue only if TI in reasonable range
                if (invTimes[iSwap] > this->MaxTIForSignInvert) continue;

                // continue if sign was already calculated before
                if (signs[iSwap] != 0) continue;

                // in each iteration change the sign of one of the signal elements
                signal[iSwap] = -fabs(signal[iSwap]);

                // start from the starting point
                this->getFunctionsT1()->copyToParameters(this->_StartPoint);

                // fit
                this->getFitter()->performFitting();
                lastValueTemp = this->getFunctionsT1()->calcCostValue();

                // are these the best tempResults?
                if (lastValueTemp < lastValue) {
                    // save the tempResults at the best tempResults
                    KWUtil::copyArrayToArray(3, tempResults, this->getFunctionsT1()->getParameters());
                    lastValue = lastValueTemp;
                }
            }


            if (lastValue != 1e32 && tempResults[0] != 0) {
                restulsStruct.T1 = tempResults[2] * (tempResults[1] / tempResults[0] - 1.);
                // tempResults.R2 = this->CalculateR2AbsFromModel(invTimes, sigMag, curPos); //TODO
                restulsStruct.A      = tempResults[0];
                restulsStruct.B      = tempResults[1];
                restulsStruct.T1star = tempResults[2];
                restulsStruct.ChiSqrt = KWUtil::getChiSqrt(lastValue, nSamples);
                restulsStruct.SNR =  (restulsStruct.B - restulsStruct.A) / (restulsStruct.ChiSqrt + 0.001);
                restulsStruct.LastValue = lastValue;
                //vecType residuals(nSamples); //TODO
                //this->m_Minimizer->GetFunctor()->calcLSResiduals(residuals); //TODO
                //vnl_matrix<MeasureType> invCovarianceMatrix = this->CalculateInvCovarianceMatrix(invTimes, residuals, curPos); //TODO
                //vnl_matrix<MeasureType> covarianceMatrix(3,3,0); //TODO
                //if (vnl_determinant(invCovarianceMatrix) > 1e-12){ //TODO
                //    covarianceMatrix = vnl_matrix_inverse<MeasureType> (invCovarianceMatrix); //TODO
                //}

                //tempResults.SD_A = covarianceMatrix(1,1); //TODO
                //tempResults.SD_B = covarianceMatrix(2,2); //TODO
                //tempResults.SD_T1 = covarianceMatrix(0,0); //TODO

                //std::cout << tempResults.SD_A << " " << tempResults.SD_B << " " << tempResults.SD_T1 << " " << std::endl;
            }

            return restulsStruct; // EXIT_SUCCESS
        }

    protected:
        double MaxTIForSignInvert;
    };

} //namespace Ox
#endif //OXSHMOLLI2_OXCALCULATORT1MOLLI_H
