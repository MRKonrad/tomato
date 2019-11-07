/*!
 * \file OxFunctionsT2ThreeParam.h
 * \author Konrad Werys
 * \date 2019/11/04
 */

#ifndef Tomato_OXFUNCTIONST2ThreeParam_H
#define Tomato_OXFUNCTIONST2ThreeParam_H

#include "tomatolib_export.h"
#include "OxFunctionsT1.h"
#include <cmath>

namespace Ox {

    /**
     * \brief Container for a ThreeParam model function \f$ A + B\exp(t/T_2) \f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT2ThreeParam : public FunctionsT1<MeasureType>{

    public:

        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time);
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals);
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian);
        virtual MeasureType calcCostValue(const MeasureType* parameters );
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative);

        FunctionsT2ThreeParam() : FunctionsT1<MeasureType>(){
            this->_nDims = 3;
        }

        /**
         * cloning
         * @return
         */
        virtual FunctionsT1<MeasureType> *newByCloning() { return new FunctionsT2ThreeParam<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT2ThreeParam(){};
    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFunctionsT2ThreeParam.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXFUNCTIONST2ThreeParam_H
