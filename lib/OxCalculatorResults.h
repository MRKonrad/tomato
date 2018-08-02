/*!
 * \file OxCalculatorT1Results.h
 * \author Konrad Werys
 * \date 2018/08/02
 */

#ifndef OXSHMOLLI2_OXCALCULATORRESULTS_H
#define OXSHMOLLI2_OXCALCULATORRESULTS_H

namespace Ox {
    template<typename MeasureType>
    struct CalculatorT1Results {

        MeasureType A;
        MeasureType B;
        MeasureType T1star;
        MeasureType T1;
        MeasureType R2;
        MeasureType ChiSqrt;
        MeasureType SNR;
        MeasureType LastValue;
        unsigned int NShmolliSamplesUsed;
        MeasureType SD_A;
        MeasureType SD_B;
        MeasureType SD_T1;

        bool hasBeenCalculated;

        /**
         * default contructor
         */
        CalculatorT1Results() {
            A = 0;
            B = 0;
            T1star = 0;
            T1 = 0;
            R2 = 0;
            ChiSqrt = 0;
            SNR = 0;
            LastValue = 1e32;
            NShmolliSamplesUsed = 0;
            SD_A = 0;
            SD_B = 0;
            SD_T1 = 0;

            hasBeenCalculated = false;
        };
    };
} // namespace Ox

#endif //OXSHMOLLI2_OXCALCULATORRESULTS_H
