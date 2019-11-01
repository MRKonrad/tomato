/*!
 * \file OxFactoryOfSignCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFSignCalculators_H
#define Tomato_OXFACTORYOFSignCalculators_H

#include "CmakeConfigForTomato.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#ifdef USE_PRIVATE_NR2
#include "OxSignCalculatorShmolli.h"
#endif

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;


    static const char *signCalculatorsTypeNames[] = {
            "NoSign",
            "RealImag",
            "MagPhase"
    };

    enum signCalculatorsType_t {
        NoSign = 0,
        RealImag = 1,
        MagPhase = 2,
        lastSignCalculatorType = MagPhase
    };

    static int signCalculatorsAvailability [] = {
            1, // NoSign
            1, // RealImag
            1, // MagPhase
    };

    template<typename TYPE>
    class FactoryOfSignCalculators {
    public:

        static SignCalculator<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            switch (opts->sign_calc_method){
                case NoSign: {
                    return new SignCalculatorNoSign<TYPE>();
                }
                case RealImag: {
                    return new SignCalculatorRealImag<TYPE>();
                }
                case MagPhase: {
                    return new SignCalculatorShmolli<TYPE>();
                }
                default:
                    throw std::runtime_error("sign_calc_method not available");
            }
        }

        static void disp(int sign_calc_method = -1){

            if (sign_calc_method >= 0) {
                printf("%-28s%-22s", " sign_calc_method: ", signCalculatorsTypeNames[sign_calc_method]);
            }

            printf("options: [ ");

            for (int i = 0; i < lastSignCalculatorType+1; i++){

                if(signCalculatorsAvailability[i]){
                    printf("%s ", signCalculatorsTypeNames[i]);
                }
            }

            printf("] \n");
        }
    };


} // namespace Ox

#endif //Tomato_OXFACTORYOFSignCalculators_H
