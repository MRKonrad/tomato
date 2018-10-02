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

#ifdef USE_PRIVATE_NR2
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
#else
    static const char *signCalculatorsTypeNames[] = {
            "NoSign",
            "RealImag"
    };

    enum signCalculatorsType_t {
        NoSign = 0,
        RealImag = 1,
        lastSignCalculatorType = RealImag
    };
#endif

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
#ifdef USE_PRIVATE_NR2
                case MagPhase: {
                    return new SignCalculatorShmolli<TYPE>();
                }
#endif
                default:
                    return 0;
            }
        }
    };

} // namespace Ox

#endif //Tomato_OXFACTORYOFSignCalculators_H
