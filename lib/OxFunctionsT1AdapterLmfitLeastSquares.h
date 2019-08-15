/*!
 * \file OxFunctionsT1AdapterLmfitLeastSquares.h
 * \author Konrad Werys
 * \date 2019/08/15
 */

#ifndef Tomato_OXFUNCTIONST1ADAPTERLMFITLEASTSQUARES_H
#define Tomato_OXFUNCTIONST1ADAPTERLMFITLEASTSQUARES_H

#include "CmakeConfigForTomato.h"
#ifdef USE_LMFIT

#include "OxFunctionsT1.h"

namespace Ox {

    template<typename TYPE>
    class FunctionsT1AdapterLmfitLeastSquares {

    public:

        // least squares function problem formulation
        static inline
        void
        calcLSResiduals(const double *parameters, int nSamples, const void *data_void, double *residuals, int *info ){

            FunctionsT1<TYPE> *Functor = const_cast<FunctionsT1<TYPE> *>(reinterpret_cast<const FunctionsT1<TYPE> *>(data_void));

            Functor->calcLSResiduals(parameters, residuals);

        }

    };

} // namespace Ox


#endif //USE_LMFIT

#endif //Tomato_OXFUNCTIONST1ADAPTERLMFITLEASTSQUARES_H
