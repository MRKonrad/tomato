/*!
 * \file OxFunctionsT1TwoParams.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXFUNCTIONST1TwoParams_H
#define Tomato_OXFUNCTIONST1TwoParams_H

#include "tomatolib_export.h"
#include "OxFunctionsT1.h"
#include <cmath>

namespace Ox {

    /**
     * \brief Container for a TwoParams model function \f$ A(1 - exp( -time / T1 )) \f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT1TwoParam : public FunctionsT1<MeasureType>{

    public:

        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time);
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals);
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian);
        virtual MeasureType calcCostValue(const MeasureType* parameters );
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative);

        FunctionsT1TwoParam() : FunctionsT1<MeasureType>(){
            this->_nDims = 2;
        }

        /**
         * cloning
         * @return
         */
        virtual FunctionsT1<MeasureType> *newByCloning() { return new FunctionsT1TwoParam<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT1TwoParam(){};
    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFunctionsT1TwoParam.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXFUNCTIONST1TwoParams_H
