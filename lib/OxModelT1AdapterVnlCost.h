/*!
 * \file OxModelT1AdapterVnlCost.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef Tomato_OXModelT1ADAPTERVNLCOST_H
#define Tomato_OXModelT1ADAPTERVNLCOST_H

#include "CmakeConfigForTomato.h"
#ifdef USE_VNL

#include <vnl/vnl_cost_function.h>

#include "OxModel.h"

namespace Ox {

    class ModelT1AdapterVnlCost : public vnl_cost_function {

    public:

        // cost function problem formulation
        double f(vnl_vector<double> const& params){ // override
            //_ModelT1->copyToParameters(params.data_block());
            return _ModelT1->calcCostValue(params.data_block());
        }

        // cost function gradient
        void gradf (vnl_vector< double > const &params, vnl_vector< double > &gradient){ // override

            //_ModelT1->copyToParameters(params.data_block());
            _ModelT1->calcCostDerivative(params.data_block(), gradient.data_block());
        }

        void setModelT1(Model<double>* _ModelT1){
            this->_ModelT1 = _ModelT1;
        };

        // getters
        Model<double>* getModelT1(){
            return _ModelT1;
        };

        /**
         * constructor
         */
        ModelT1AdapterVnlCost(int nDims) : vnl_cost_function(nDims){
            _ModelT1 = 0; // nullpointer
        };

        /**
         * copy constructor
         * @param old
         */
        ModelT1AdapterVnlCost(const ModelT1AdapterVnlCost &old){
            _ModelT1 = old._ModelT1;
        }

    private:

        Model<double>* _ModelT1;
    };

} // namespace Ox

#endif //USE_VNL

#endif //Tomato_OXModelT1ADAPTERVNLCOST_H
