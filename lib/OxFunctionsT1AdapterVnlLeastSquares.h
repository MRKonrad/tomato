/*!
 * \file OxFunctionsT1AdapterVnlLeastSquares.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLLEASTSQUARES_H
#define OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLLEASTSQUARES_H

#include <vnl/vnl_least_squares_function.h>

#include "OxFunctionsT1.h"

namespace Ox {

    class FunctionsT1AdapterVnlLeastSquares : public vnl_least_squares_function {

    public:

        // least squares function problem formulation
        void f(vnl_vector<double> const& params, vnl_vector<double> &residuals){

            _FunctionsT1->copyToParameters(params.data_block());
            _FunctionsT1->calcLSResiduals(residuals.data_block());
        }

        // least squares function gradient
        void gradf (vnl_vector< double > const &params, vnl_matrix< double > &jacobianVNL){

            _FunctionsT1->copyToParameters(params.data_block());
            _FunctionsT1->calcLSJacobian(jacobianVNL.data_block());
        }

        void setFunctionsT1(FunctionsT1<double>* _FunctionsT1){
            this->_FunctionsT1 = _FunctionsT1;
        };

        // getters
        FunctionsT1<double>* getFunctionsT1(){
            return _FunctionsT1;
        };

        /**
         * constructor
         */
        FunctionsT1AdapterVnlLeastSquares(int nDims, int nSamples, UseGradient UseGradientVariable) : vnl_least_squares_function (nDims, nSamples, UseGradientVariable){
            _FunctionsT1 = 0; // nullpointer
        };

        /**
         * copy constructor
         * @param old
         */
        FunctionsT1AdapterVnlLeastSquares(const FunctionsT1AdapterVnlLeastSquares &old) : vnl_least_squares_function(old){
            _FunctionsT1 = old._FunctionsT1;
        }

    private:

        FunctionsT1<double>* _FunctionsT1;
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLLEASTSQUARES_H
