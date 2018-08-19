/*!
 * \file OxFactoryOfFitters.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef OXSHMOLLI2_OXFACTORYOFFitters_H
#define OXSHMOLLI2_OXFACTORYOFFitters_H

#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"

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

        static Fitter<TYPE>* generateFittersObject(OxShmolli2Options<TYPE> *opts){
            switch (opts->fitting_method){
                case AmoebaVnl: {
                    return new FitterAmoebaVnl<TYPE>();
                }
                case LevMarVnl: {
                    return new FitterLevenbergMarquardtVnl<TYPE>();
                }
                default:
                    return 0;
            }
        }
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFACTORYOFFitters_H
