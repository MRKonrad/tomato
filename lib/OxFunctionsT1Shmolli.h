/*!
 * \file OxFunctionsT1Shmolli.h
 * \author Konrad Werys
 * \date 2018/08/22
 */

#ifndef Tomato_OXFunctionsT1Shmolli_H
#define Tomato_OXFunctionsT1Shmolli_H

#include "tomatolib_export.h"
#include "OxFunctionsT1.h"
#include <cmath>

namespace Ox {

    /**
     * \class FunctionsT1Shmolli
     * \brief Container for a CalculatorShmolli model function \f$ A-B\exp(t/T_1^*) \f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT1Shmolli : public FunctionsT1<MeasureType>{

    public:

        bool _expAbsCost;
        bool _preventUnderOverFlow;
        bool _costHeuristic;
        bool _rootMedianSquareCost;

        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time);
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals);
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian);
        virtual MeasureType calcCostValue(const MeasureType* parameters );
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative);

        FunctionsT1Shmolli() : FunctionsT1<MeasureType>(){
            _expAbsCost = true;
            _preventUnderOverFlow = true;
            _costHeuristic = true;
            _rootMedianSquareCost = false;

            this->_nDims = 3;
        }

        /**
         * cloning
         * @return
         */
        virtual FunctionsT1<MeasureType> *newByCloning() { return new FunctionsT1Shmolli<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT1Shmolli(){};
    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFunctionsT1Shmolli.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXFunctionsT1Shmolli_H
