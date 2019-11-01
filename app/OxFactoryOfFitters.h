/*!
 * \file OxFactoryOfFitters.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFFitters_H
#define Tomato_OXFACTORYOFFitters_H

#include "CmakeConfigForTomato.h"
#include "TomatoOptions.h"

#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#ifdef USE_PRIVATE_NR2
#include "OxFitterAmoebaPrivateNr2.h"
#endif
#ifdef USE_LMFIT
#include "OxFitterLevenbergMarquardtLmfit.h"
#endif


namespace Ox {

    /**
     * Here you can configure different fitting methods
     * @tparam TYPE
     */
    template<typename TYPE>
    struct TomatoOptions;

    static const char *fittersTypeNames[] = {
            "AmoebaVnl",
            "LevMarVnl",
            "AmoebaPrivateNr2",
            "LevMarLmfit"
    };

    enum fittersType_t {
        AmoebaVnl = 0,
        LevMarVnl = 1,
        AmoebaPrivateNr2 = 2,
        LevMarLmfit = 3,
        lastFitterType = LevMarLmfit
    };

    static int fittersAvailability[] = {
            1, // AmoebaVnl
            1, // LevMarVnl
#ifdef USE_PRIVATE_NR2
            1, // AmoebaPrivateNr2
#else
            0, // AmoebaPrivateNr2
#endif

#ifdef USE_LMFIT
            1 // LevMarLmfit
#else
            0 // LevMarLmfit
#endif
    };

    template<typename TYPE>
    class FactoryOfFitters {
    public:

        static Fitter<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
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

#ifdef USE_LMFIT
                case LevMarLmfit: {
                    fitter = new FitterLevenbergMarquardtLmfit<TYPE>();
                    break;
                }
#endif
                default:
                    throw std::runtime_error("fitting_method not available");
            }

            fitter->setMaxFunctionEvals(opts->max_function_evals);
            fitter->setFTolerance(opts->fTolerance);
            fitter->setUseGradient(opts->use_gradient);

            return fitter;
        }

        static void disp(int fitting_method = -1){

            if (fitting_method >= 0) {
                printf("%-28s%-22s", " fitting_method: ", fittersTypeNames[fitting_method]);
            }

            printf("options: [ ");

            for (int i = 0; i < lastFitterType+1; i++){

                if(fittersAvailability[i]){
                    printf("%s ", fittersTypeNames[i]);
                }
            }

            printf("] \n");
        }

    };

} // namespace Ox

#endif //Tomato_OXFACTORYOFFitters_H
