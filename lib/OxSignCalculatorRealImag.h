/*!
 * \file OxSignCalculatorRealImag.h
 * \author Konrad Werys
 * \date 2018/08/01
 */

#ifndef OXSHMOLLI2_OXSIGNCALCULATORRealImag_H
#define OXSHMOLLI2_OXSIGNCALCULATORRealImag_H

#include "OxSignCalculator.h"
#include "KWUtil.h"

namespace Ox {

    /**
     * \class SignCalculatorRealImag
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class SignCalculatorRealImag : public SignCalculator<MeasureType> {

    public:
        virtual int calculateSign(){
            return RealMagPhase2Signs(
                    this->getNSamples(),
                    this->getSigMag(),
                    this->getSigPha(),
                    this->getSignal(),
                    this->getSigns());
        };

        static int RealMagPhase2Signs(
                int nSamples,
                const MeasureType* sigMag,
                const MeasureType* sigPha,
                MeasureType* signal,
                MeasureType* signs);

        /**
         * cloning
         * @return
         */
        virtual SignCalculator<MeasureType> *newByCloning() { return new SignCalculatorRealImag<MeasureType>(*this); }

    };

    template<typename MeasureType>
    int
    SignCalculatorRealImag<MeasureType>
    ::RealMagPhase2Signs(
            int nSamples,
            const MeasureType* sigMag,
            const MeasureType* sigPha,
            MeasureType* signal,
            MeasureType* signs) {

//        for (int i = 0; i < nSamples; ++i) {
//            signal[i] = sigMag[i];
//            signs[i] = 0;
//        }
        //TODO: introduce arbitrary scaling, not only /4096

        if (sigPha) { // if phase available

            double sigPhaRefPi = M_PI * sigPha[nSamples - 1] / 4096; //get REFERENCE phase

            for (int i = 0; i < nSamples; ++i) {

                double sigPhaPi = M_PI * sigPha[i] / 4096; //get phase
                double pha = sigPhaPi - sigPhaRefPi;
                double realSig = sigMag[i] * cos(pha); //get real
                double imagSig = -sigMag[i] * sin(pha); //get imag
                //signal[i] = realSig;
                if (realSig < 0) {
                    signal[i] = -sigMag[i];
                    signs[i] = -1;
                } else {
                    signal[i] = sigMag[i];
                    signs[i] = 1;
                }
            }
        } else {
            return 1; // failure
        }
        return 0; // success
    }
} //namespace Ox

#endif //OXSHMOLLI2_OXSIGNCALCULATOR_H
