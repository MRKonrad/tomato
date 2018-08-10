/*!
 * \file OxFunctionsT1AdapterVnlCost.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLCOST_H
#define OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLCOST_H

#include <vnl/vnl_cost_function.h>

#include "OxFunctionsT1.h"

namespace Ox {

    class FunctionsT1AdapterVnlCost : public vnl_cost_function {

    public:

        // cost function problem formulation
        double f(vnl_vector<double> const& params){ // override
            _FunctionsT1->copyToParameters(params.data_block());
            return _FunctionsT1->calcCostValue();
        }

        // cost function gradient
        void gradf (vnl_vector< double > const &params, vnl_vector< double > &gradient){ // override

            _FunctionsT1->copyToParameters(params.data_block());
            _FunctionsT1->calcCostDerivative(gradient.data_block());
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
        FunctionsT1AdapterVnlCost(int nDims) : vnl_cost_function(nDims){
            _FunctionsT1 = 0; // nullpointer
        };

        /**
         * copy constructor
         * @param old
         */
        FunctionsT1AdapterVnlCost(const FunctionsT1AdapterVnlCost &old){
            _FunctionsT1 = old._FunctionsT1;
        }

    private:

        FunctionsT1<double>* _FunctionsT1;
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLCOST_H
