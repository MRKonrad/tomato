/*!
 * \file OxFactoryOfSignCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFSignCalculators_H
#define Tomato_OXFACTORYOFSignCalculators_H

#include "CmakeConfigForTomato.h"
#include "tomatolib_export.h"
#include "OxSignCalculator.h"

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;


    static const char *signCalculatorsTypeNames[] = {
            "NoSign",
            "RealImag",
            "MagPhase",
            "None"
    };

    enum signCalculatorsType_t {
        NoSign = 0,
        RealImag = 1,
        MagPhase = 2,
        None = 3,
        lastSignCalculatorType = None
    };

    static int signCalculatorsAvailability [] = {
            1, // NoSign
            1, // RealImag
            1, // MagPhase
            1, // None
    };

    template<typename TYPE>
    class FactoryOfSignCalculators {
    public:

        static SignCalculator<TYPE>* newByFactory(TomatoOptions<TYPE> *opts);

        static void disp(int sign_calc_method = -1);
    };


} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFactoryOfSignCalculators.hxx"
#endif // TOMATOLIB_COMPILED

#endif //Tomato_OXFACTORYOFSignCalculators_H
