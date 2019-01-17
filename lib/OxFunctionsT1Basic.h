/*!
 * \file OxFunctionsT1Basic.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXFUNCTIONST1BASIC_H
#define Tomato_OXFUNCTIONST1BASIC_H

#include "OxFunctionsT1.h"
#include <cmath>

namespace Ox {

    /**
     * \class FunctionsT1Basic
     * \brief Container for a basic model function \f$ A-B\exp(t/T_1^*) \f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT1Basic : public FunctionsT1<MeasureType>{

    public:

        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time);
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals);
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian);
        virtual MeasureType calcCostValue(const MeasureType* parameters );
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative);

        FunctionsT1Basic() : FunctionsT1<MeasureType>(){
            this->_nDims = 3;
        }

        /**
         * cloning
         * @return
         */
        virtual FunctionsT1<MeasureType> *newByCloning() { return new FunctionsT1Basic<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT1Basic(){};
    };
} //namespace Ox

#ifdef TomatoLib_EXPORTS
#include "OxFunctionsT1Basic.hxx"
#endif //TomatoLib_EXPORTS

#endif //Tomato_OXFUNCTIONST1BASIC_H
