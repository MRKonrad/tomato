/*!
 * \file OxModelT1AdapterLmfitLeastSquares.h
 * \author Konrad Werys
 * \date 2019/08/15
 */

#ifndef Tomato_OXModelT1ADAPTERLMFITLEASTSQUARES_H
#define Tomato_OXModelT1ADAPTERLMFITLEASTSQUARES_H

#include "CmakeConfigForTomato.h"
#ifdef USE_LMFIT

#include "OxModel.h"

namespace Ox {

    template<typename TYPE>
    class ModelT1AdapterLmfitLeastSquares {

    public:

        // least squares function problem formulation
        static inline
        void
        calcLSResiduals(const double *parameters, int nSamples, const void *data_void, double *residuals, int *info ){

            Model<TYPE> *model = const_cast<Model<TYPE> *>(reinterpret_cast<const Model<TYPE> *>(data_void));

            model->calcLSResiduals(parameters, residuals);

        }

    };

} // namespace Ox


#endif //USE_LMFIT

#endif //Tomato_OXModelT1ADAPTERLMFITLEASTSQUARES_H
