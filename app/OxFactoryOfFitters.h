/*!
 * \file OxFactoryOfFitters.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef OXSHMOLLI2_OXFACTORYOFFitters_H
#define OXSHMOLLI2_OXFACTORYOFFitters_H

#include "CmakeConfigForOxShmolli2.h"
#include "OxShmolli2Options.h"

#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#ifdef USE_PRIVATE_NR2
#include "OxFitterAmoebaPrivateNr2.h"
#endif


namespace Ox {

    template<typename TYPE>
    struct OxShmolli2Options;


#ifdef USE_PRIVATE_NR2
    static const char *fittersTypeNames[] = {
            "AmoebaVnl",
            "LevMarVnl",
            "AmoebaPrivateNr2"
    };

    enum fittersType_t {
        AmoebaVnl = 0,
        LevMarVnl = 1,
        AmoebaPrivateNr2 = 2,
        lastFitterType = AmoebaPrivateNr2
    };
#else
    static const char *fittersTypeNames[] = {
                "AmoebaVnl",
                "LevMarVnl"
        };

        enum fittersType_t {
            AmoebaVnl = 0,
            LevMarVnl = 1,
            lastFitterType = LevMarVnl
        };
#endif

    template<typename TYPE>
    class FactoryOfFitters {
    public:

        static Fitter<TYPE>* newByFactory(OxShmolli2Options<TYPE> *opts){
            Fitter<TYPE> *fitter = 0; //nullpointer

            switch (opts->fitting_method){
                case AmoebaVnl: {
                    fitter = new FitterAmoebaVnl<TYPE>();
                    break;
                }
                case LevMarVnl: {
                    fitter = new FitterLevenbergMarquardtVnl<TYPE>();
                    break;
                }
#ifdef USE_PRIVATE_NR2
                case AmoebaPrivateNr2: {
                    fitter = new FitterAmoebaPrivateNr2<TYPE>();
                    break;
                }
#endif
            }

            fitter->setMaxFunctionEvals(opts->max_function_evals);
            fitter->setFTolerance(opts->fTolerance);

            return fitter;
        }
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFACTORYOFFitters_H
