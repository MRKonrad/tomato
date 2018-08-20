/*!
 * \file OxFactoryOfCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef OXSHMOLLI2_OXFACTORYOFCalculators_H
#define OXSHMOLLI2_OXFACTORYOFCalculators_H

#include "OxCalculatorT1Molli.h"
// #include "OxCalculatorT1Shmolli.h"


namespace Ox {

    template<typename TYPE>
    struct OxShmolli2Options;

    static const char *calculatorsTypeNames[] = {
            "T1_MOLLI",
    };

    enum calculatorsType_t {
        T1_MOLLI = 0,
        lastCalculatorType = T1_MOLLI
    };

    template<typename TYPE>
    class FactoryOfCalculators {
    public:

        static CalculatorT1<TYPE>* newByFactory(OxShmolli2Options<TYPE> *opts){
            switch (opts->parameter_to_map){
                case T1_MOLLI: {
                    return new CalculatorT1Molli<TYPE>();
                }
                default:
                    return 0;
            }
        }
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFACTORYOFCalculators_H
