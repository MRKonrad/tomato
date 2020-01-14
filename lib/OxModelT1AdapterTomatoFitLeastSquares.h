/*!
 * \file OxModelT1AdapterTomatoFitLeastSquares.h
 * \author Konrad Werys
 * \date 2020/01/13
 */

#ifndef Tomato_OXModelT1AdapterTomatoFitLeastSquares_H
#define Tomato_OXModelT1AdapterTomatoFitLeastSquares_H

#include "CmakeConfigForTomato.h"
#ifdef USE_TOMATOFIT

#include "OxModel.h"
#include "TFModel.h"

namespace Ox {

    template<typename TYPE>
    class ModelT1AdapterTomatoFitLeastSquares : public TFModel<TYPE> {

    public:

        double getValue(const double *params, double t) {
            return _Model->calcModelValue(params, t);
        }

        Model <TYPE> *getModel() const {
            return _Model;
        }

        void setModel(Model <TYPE> *model) {
            _Model = model;
            this->_nDims = _Model->getNDims();
        }

    private:
        Model<TYPE> *_Model;

    };

} // namespace Ox


#endif //USE_TOMATOFIT

#endif //Tomato_OXModelT1AdapterTomatoFitLeastSquares_H
