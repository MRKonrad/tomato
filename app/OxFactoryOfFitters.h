/*!
 * \file OxFactoryOfFitters.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef OXSHMOLLI2_OXFACTORYOFFitters_H
#define OXSHMOLLI2_OXFACTORYOFFitters_H

#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxShmolli2Options.h"

namespace Ox {

    template<typename TYPE>
    struct OxShmolli2Options;

    static const char *fittersTypeNames[] = {
            "AmoebaVnl",
            "LevMarVnl"
    };

    enum fittersType_t {
        AmoebaVnl = 0,
        LevMarVnl = 1,
        lastFitterType = LevMarVnl
    };

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
            }

            fitter->setMaxFunctionEvals(opts->max_function_evals);
            fitter->setFTolerance(opts->fTolerance);

            return fitter;
        }
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFACTORYOFFitters_H
