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

        FunctionsT1AdapterVnlLeastSquares(int nSamples, UseGradient UseGradientVariable) : vnl_least_squares_function (3, nSamples, UseGradientVariable){
            _FunctionsT1 = 0; // nullpointer
        };

        // least squares function problem formulation
        void f(vnl_vector<double> const& params, vnl_vector<double> &residuals){

            double tempParams[3];
            params.copy_out(tempParams);
            _FunctionsT1->setParameters(tempParams);
            _FunctionsT1->calcLSResiduals(residuals.data_block());
        }

        // least squares function gradient
        void gradf (vnl_vector< double > const &params, vnl_matrix< double > &jacobianVNL){

            double tempParams[3];
            params.copy_out(tempParams);
            _FunctionsT1->setParameters(tempParams);
            _FunctionsT1->calcLSJacobian(jacobianVNL.data_block());
        }

        void setFunctionsT1(FunctionsT1<double>* _FunctionsT1){
            this->_FunctionsT1 = _FunctionsT1;
        };

        FunctionsT1<double>* getFunctionsT1(){
            return _FunctionsT1;
        };

    private:

        FunctionsT1<double>* _FunctionsT1;
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLLEASTSQUARES_H
