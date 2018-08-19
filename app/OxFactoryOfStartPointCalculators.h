/*!
 * \file OxFactoryOfStartPointCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef OXSHMOLLI2_OXFACTORYOFStartPointCalculators_H
#define OXSHMOLLI2_OXFACTORYOFStartPointCalculators_H

#include "OxStartPointCalculatorDefault3Dims.h"

namespace Ox {

    template<typename TYPE>
    struct OxShmolli2Options;

    static const char *startPointCalculatorsTypeNames[] = {
            "Default",
    };

    enum startPointCalculatorsType_t {
        Default = 0,
        lastStartPointCalculatorType = Default
    };

    template<typename TYPE>
    class FactoryOfStartPointCalculators {
    public:

        static StartPointCalculator<TYPE>* generateStartPointCalculator(OxShmolli2Options<TYPE> *opts){
            switch (opts->start_point_calc_method){
                case Default: {
                    return new StartPointCalculatorDefault3Dims<TYPE>();
                }
                default:
                    return 0;
            }
        }
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFACTORYOFStartPointCalculators_H
