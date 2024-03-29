/*!
 * \file OxFactoryOfFitters.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFFitters_H
#define Tomato_OXFACTORYOFFitters_H

#include "CmakeConfigForTomato.h"
#include "tomatolib_export.h"

#include "OxFitter.h"


namespace Ox {

    /**
     * Here you can configure different fitting methods
     * @tparam TYPE
     */
    template<typename TYPE>
    class TomatoOptions;

    static const char *fittersTypeNames[] = {
            "AmoebaVnl",
            "LevMarVnl",
            "AmoebaPrivateNr2",
            "LevMarLmfit",
            "LevMarTomatoFit",
            "RobustTomatoFit"
    };

    enum fittersType_t {
        AmoebaVnl = 0,
        LevMarVnl = 1,
        AmoebaPrivateNr2 = 2,
        LevMarLmfit = 3,
        LevMarTomatoFit = 4,
        RobustTomatoFit = 5,
        lastFitterType = RobustTomatoFit
    };

    static int fittersAvailability[] = {
#ifdef USE_VNL
            1, // AmoebaVnl
            1, // LevMarVnl
#else
            0, // AmoebaVnl
            0, // LevMarVnl
#endif
#ifdef USE_PRIVATE_NR2
            1, // AmoebaPrivateNr2
#else
            0, // AmoebaPrivateNr2
#endif

#ifdef USE_LMFIT
            1, // LevMarLmfit
#else
            0, // LevMarLmfit
#endif
#ifdef USE_TOMATOFIT
            1, // LevMarTomatoFit
            1  // RobustTomatoFit
#else
            0, // LevMarTomatoFit
            0  // RobustTomatoFit
#endif
    };

    template<typename TYPE>
    class FactoryOfFitters {
    public:

        static Fitter<TYPE>* newByFactory(TomatoOptions<TYPE> *opts);

        static void disp(int fitting_method = -1);

    };

} // namespace Ox


#ifndef TOMATOLIB_COMPILED
#include "OxFactoryOfFitters.hxx"
#endif // TOMATOLIB_COMPILED

#endif //Tomato_OXFACTORYOFFitters_H
