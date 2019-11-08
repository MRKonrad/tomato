/*!
 * \file OxModelT1Shmolli.h
 * \author Konrad Werys
 * \date 2018/08/22
 */

#ifndef Tomato_OXModelT1Shmolli_H
#define Tomato_OXModelT1Shmolli_H

#include "tomatolib_export.h"
#include "OxModel.h"
#include <cmath>

namespace Ox {

    /**
     * \class ModelT1Shmolli
     * \brief Container for a CalculatorShmolli model function \f$ A-B\exp(t/T_1^*) \f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class ModelT1Shmolli : public Model<MeasureType>{

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

        ModelT1Shmolli() : Model<MeasureType>(){
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
        virtual Model<MeasureType> *newByCloning() { return new ModelT1Shmolli<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~ModelT1Shmolli(){};
    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxModelT1Shmolli.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXModelT1Shmolli_H
