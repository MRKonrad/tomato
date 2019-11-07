/*!
 * \file OxFunctionsT1ThreeParam.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXFUNCTIONST1ThreeParam_H
#define Tomato_OXFUNCTIONST1ThreeParam_H

#include "tomatolib_export.h"
#include "OxFunctionsT1.h"
#include <cmath>

namespace Ox {

    /**
     * \brief Container for a ThreeParam model function \f$ A-B\exp(t/T_1^*) \f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT1ThreeParam : public FunctionsT1<MeasureType>{

    public:

        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time);
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals);
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian);
        virtual MeasureType calcCostValue(const MeasureType* parameters );
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative);

        FunctionsT1ThreeParam() : FunctionsT1<MeasureType>(){
            this->_nDims = 3;
        }

        /**
         * cloning
         * @return
         */
        virtual FunctionsT1<MeasureType> *newByCloning() { return new FunctionsT1ThreeParam<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT1ThreeParam(){};
    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFunctionsT1ThreeParam.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXFUNCTIONST1ThreeParam_H
