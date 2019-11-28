/*!
 * \file OxFactoryOfStartPointCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFStartPointCalculators_H
#define Tomato_OXFACTORYOFStartPointCalculators_H

#include "CmakeConfigForTomato.h"
#include "tomatolib_export.h"

#include "OxStartPointCalculator.h"

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;

    static const char *startPointCalculatorsTypeNames[] = {
            "Basic",
            "StartPointSHMOLLI",
            "NoStartPointCalculators"
    };

    enum startPointCalculatorsType_t {
        Basic = 0,
        StartPointSHMOLLI = 1,
        NoStartPointCalculators = 2,
        lastStartPointCalculatorType = NoStartPointCalculators
    };

    static int startPointCalculatorsAvailability[] = {
            1, // Basic
            1, // StartPointSHMOLLI
            1  // NoStartPointCalculators
    };

    template<typename TYPE>
    class FactoryOfStartPointCalculators {
    public:

        static StartPointCalculator<TYPE>* newByFactory(TomatoOptions<TYPE> *opts);

        static void disp(int start_point_calc_method = -1);
    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFactoryOfStartPointCalculators.hxx"
#endif // TOMATOLIB_COMPILED

#endif //Tomato_OXFACTORYOFStartPointCalculators_H
