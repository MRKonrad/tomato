/*!
 * \file OxFunctionsT1Basic.hxx
 * \author Konrad Werys
 * \date 2018/08/29
 */

#ifndef OXSHMOLLI2_OXFUNCTIONST1BASIC_HXX
#define OXSHMOLLI2_OXFUNCTIONST1BASIC_HXX

namespace Ox {


    template< typename MeasureType >
    MeasureType
    FunctionsT1Basic<MeasureType>
    ::calcModelValue(MeasureType time){
        MeasureType A = this->_Parameters[0];
        MeasureType B = this->_Parameters[1];
        MeasureType T1star = this->_Parameters[2];

        return A - B * exp( -time / T1star );
    }

    template< typename MeasureType >
    void
    FunctionsT1Basic<MeasureType>
    ::calcLSResiduals(MeasureType* residuals){

    }

    template< typename MeasureType >
    void
    FunctionsT1Basic<MeasureType>
    ::calcLSJacobian(MeasureType** jacobian){

    }

    template< typename MeasureType >
    MeasureType
    FunctionsT1Basic<MeasureType>
    ::calcCostValue(){
        return 0;
    }

    template< typename MeasureType >
    void
    FunctionsT1Basic<MeasureType>
    ::calcCostDerivative(MeasureType* derivative){

    }

} //namespace Ox


#endif //OXSHMOLLI2_OXFUNCTIONST1BASIC_HXX
