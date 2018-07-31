/*!
 * \file OxFunctionsT1AdapterVnlCost.h
 * \author Konrad Werys
 * \date 2018/08/30
 */

#ifndef OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLCOST_H
#define OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLCOST_H

#include <vnl/vnl_cost_function.h>

#include "OxFunctionsT1.h"

namespace Ox {

    class FunctionsT1AdapterVnlCost : public vnl_cost_function {

    public:

        FunctionsT1AdapterVnlCost() : vnl_cost_function(3){
            _FunctionsT1 = 0; // nullpointer
        };

        // cost function problem formulation
        double f(vnl_vector<double> const& params){ // override

            double tempParams[3];
            params.copy_out(tempParams);
            _FunctionsT1->setParameters(tempParams);
            return _FunctionsT1->calcCostValue();
        }

        // cost function gradient
        void gradf (vnl_vector< double > const &params, vnl_vector< double > &gradient){ // override

            double tempParams[3];
            params.copy_out(tempParams);
            _FunctionsT1->setParameters(tempParams);
            _FunctionsT1->calcCostDerivative(gradient.data_block());
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

#endif //OXSHMOLLI2_OXFUNCTIONST1ADAPTERVNLCOST_H
