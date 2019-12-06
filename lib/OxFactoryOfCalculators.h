/*!
 * \file OxFactoryOfCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFCalculators_H
#define Tomato_OXFACTORYOFCalculators_H

#include "CmakeConfigForTomato.h"
#include "tomatolib_export.h"
#include "OxCalculator.h"

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;

    enum paramType_t {
        T1 = 0,
        T2 = 1,
        T2star = 2,
        Perf = 3
    };

    static const char *calculatorsTypeNames[] = {
            "T1_MOLLI",
            "T1_SHMOLLI",
            "T1_SHMOLLI_original",
            "T2",
            "T2_truncation"
    };

    enum calculatorsType_t {
        T1_MOLLI = 0,
        T1_SHMOLLI = 1,
        T1_SHMOLLI_original = 2,
        T2_basic = 3,
        T2_truncation = 4,
        lastCalculatorType = T2_truncation
    };

    static int calculatorsAvailability[] = {
            1, // T1_MOLLI
            1, // T1_SHMOLLI
#ifdef USE_PRIVATE_NR2
            1, // T1_SHMOLLI_original
            1, // T2_basic
            1  // T2_truncation
#else
            0, // T1_SHMOLLI_original
            0, // T2_basic
            0  // T2_truncation
#endif

    };

    static int calculatorsParamsToCalculate[] = {
            T1, // T1_MOLLI
            T1, // T1_SHMOLLI
            T1, // T1_SHMOLLI_original
            T2, // T2_basic
            T2  // T2_truncation
    };

    template<typename TYPE>
    class FactoryOfCalculators {
    public:

        static Calculator<TYPE>* newByFactory(TomatoOptions<TYPE> *opts);

        static void disp(int parameter_to_map = -1);
    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFactoryOfCalculators.hxx"
#endif // TOMATOLIB_COMPILED

#endif //Tomato_OXFACTORYOFCalculators_H
