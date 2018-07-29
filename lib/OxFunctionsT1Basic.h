/*!
 * \file OxFunctionsT1Basic.h
 * \author Konrad Werys
 * \date 2018/08/29
 */

#ifndef OXSHMOLLI2_OXFUNCTIONST1BASIC_H
#define OXSHMOLLI2_OXFUNCTIONST1BASIC_H

#include "OxFunctionsT1.h"
#include <cmath>

namespace Ox {

    /**
     * \class FunctionsT1Basic
     * \brief Container for a basic model function \f$\sqrt{A-B\exp(t/T_1^*)}\f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT1Basic : public FunctionsT1<MeasureType>{

    public:

        virtual MeasureType calcModelValue(MeasureType time);
        virtual void calcLSResiduals(MeasureType* residuals);
        virtual void calcLSJacobian(MeasureType** jacobian);
        virtual MeasureType calcCostValue();
        virtual void calcCostDerivative(MeasureType* derivative);

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        ~FunctionsT1Basic(){};
    };
} //namespace Ox

#include "OxFunctionsT1Basic.hxx"

#endif //OXSHMOLLI2_OXFUNCTIONST1BASIC_H
