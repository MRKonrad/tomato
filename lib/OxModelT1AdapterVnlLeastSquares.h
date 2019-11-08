/*!
 * \file OxModelT1AdapterVnlLeastSquares.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef Tomato_OXModelT1ADAPTERVNLLEASTSQUARES_H
#define Tomato_OXModelT1ADAPTERVNLLEASTSQUARES_H

#include "CmakeConfigForTomato.h"
#ifdef USE_VNL

#include <vnl/vnl_least_squares_function.h>

#include "OxModel.h"

namespace Ox {

    class ModelT1AdapterVnlLeastSquares : public vnl_least_squares_function {

    public:

        // least squares function problem formulation
        void f(vnl_vector<double> const& params, vnl_vector<double> &residuals){

            //_Model->copyToParameters(params.data_block());
            _ModelT1->calcLSResiduals(params.data_block(), residuals.data_block());
        }

        // least squares function gradient
        void gradf (vnl_vector< double > const &params, vnl_matrix< double > &jacobianVNL){

            //_Model->copyToParameters(params.data_block());
            _ModelT1->calcLSJacobian(params.data_block(), jacobianVNL.data_block());
        }

        void setModel(Model<double>* _ModelT1){
            this->_ModelT1 = _ModelT1;
        };

        // getters
        Model<double>* getModel(){
            return _ModelT1;
        };

        /**
         * constructor
         */
        ModelT1AdapterVnlLeastSquares(int nDims, int nSamples, UseGradient UseGradientVariable)
                : vnl_least_squares_function (nDims, nSamples, UseGradientVariable){
            _ModelT1 = 0; // nullpointer
        };

        /**
         * copy constructor
         * @param old
         */
        ModelT1AdapterVnlLeastSquares(const ModelT1AdapterVnlLeastSquares &old)
                : vnl_least_squares_function(old){
            _ModelT1 = old._ModelT1;
        }

    private:

        Model<double>* _ModelT1;
    };

} // namespace Ox

#endif //USE_VNL

#endif //Tomato_OXModelT1ADAPTERVNLLEASTSQUARES_H
