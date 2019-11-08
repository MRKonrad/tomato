/*!
 * \file OxModelT1TwoParam.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXModelT1TwoParam_H
#define Tomato_OXModelT1TwoParam_H

#include "tomatolib_export.h"
#include "OxModel.h"
#include <cmath>

namespace Ox {

    /**
     * \brief Container for a TwoParam model function \f$ A(1 - exp( -time / T1 )) \f$, cost function and Least-Squares
     * function and derivatives.
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class ModelT1TwoParam : public Model<MeasureType>{

    public:

        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time);
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals);
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian);
        virtual MeasureType calcCostValue(const MeasureType* parameters );
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative);

        ModelT1TwoParam() : Model<MeasureType>(){
            this->_nDims = 2;
        }

        /**
         * cloning
         * @return
         */
        virtual Model<MeasureType> *newByCloning() { return new ModelT1TwoParam<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~ModelT1TwoParam(){};
    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxModelT1TwoParam.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXModelT1TwoParam_H
