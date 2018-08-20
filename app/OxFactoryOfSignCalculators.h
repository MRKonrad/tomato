/*!
 * \file OxFactoryOfSignCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef OXSHMOLLI2_OXFACTORYOFSignCalculators_H
#define OXSHMOLLI2_OXFACTORYOFSignCalculators_H

#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"

namespace Ox {

    template<typename TYPE>
    struct OxShmolli2Options;

    static const char *signCalculatorsTypeNames[] = {
            "NoSign",
            "RealImag"
    };

    enum signCalculatorsType_t {
        NoSign = 0,
        RealImag = 1,
        lastSignCalculatorType = RealImag
    };

    template<typename TYPE>
    class FactoryOfSignCalculators {
    public:

        static SignCalculator<TYPE>* newByFactory(OxShmolli2Options<TYPE> *opts){
            switch (opts->sign_calc_method){
                case NoSign: {
                    return new SignCalculatorNoSign<TYPE>();
                }
                case RealImag: {
                    return new SignCalculatorRealImag<TYPE>();
                }
                default:
                    return 0;
            }
        }
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFACTORYOFSignCalculators_H
