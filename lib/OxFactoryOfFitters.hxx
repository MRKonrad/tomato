/*!
 * \file OxFactoryOfFitters.hxx
 * \author Konrad Werys
 * \date 2019/11/26
 */

#ifndef Tomato_OXFACTORYOFFitters_HXX
#define Tomato_OXFACTORYOFFitters_HXX

#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#ifdef USE_PRIVATE_NR2
#include "OxFitterAmoebaPrivateNr2.h"
#endif
#ifdef USE_LMFIT
#include "OxFitterLevenbergMarquardtLmfit.h"
#endif


namespace Ox {

    template < typename TYPE >
    Fitter<TYPE>*
    FactoryOfFitters<TYPE>
    ::newByFactory(TomatoOptions<TYPE> *opts){
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

    template < typename TYPE >
    void
    FactoryOfFitters<TYPE>
    ::disp(int fitting_method){

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

} // namespace Ox

#endif //Tomato_OXFACTORYOFFitters_HXX
