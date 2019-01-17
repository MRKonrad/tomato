/*!
 * \file OxCalculatorT1Results.h
 * \author Konrad Werys
 * \date 2018/08/02
 */

#ifndef Tomato_OXCALCULATORRESULTS_H
#define Tomato_OXCALCULATORRESULTS_H

#include <iostream>

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
         * constructor
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

        void disp(){
            std::cout << std::endl;
            std::cout << "CalculatorT1Results: " << this << std::endl;
            std::cout << "A: " << A << std::endl;
            std::cout << "B: " << B << std::endl;
            std::cout << "T1star: " << T1star << std::endl;
            std::cout << "T1: " << T1 << std::endl;
            std::cout << "R2: " << R2 << std::endl;
            std::cout << "ChiSqrt: " << ChiSqrt << std::endl;
            std::cout << "SNR: " << SNR << std::endl;
            std::cout << "LastValue: " << LastValue << std::endl;
            std::cout << "NShmolliSamplesUsed: " << NShmolliSamplesUsed << std::endl;
            std::cout << "SD_A: " << SD_A << std::endl;
            std::cout << "SD_B: " << SD_B << std::endl;
            std::cout << "SD_T1: " << SD_T1 << std::endl;
            if (hasBeenCalculated) std::cout << "has been calculated" << std::endl;
            else std::cout << "has NOT been calculated" << std::endl;

            hasBeenCalculated = false;
        }

    };
} // namespace Ox

#endif //Tomato_OXCALCULATORRESULTS_H
