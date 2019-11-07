/*!
 * \file OxFunctionsT1TwoParam.hxx
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXFUNCTIONST1TwoParam_HXX
#define Tomato_OXFUNCTIONST1TwoParam_HXX

namespace Ox {


    template< typename MeasureType >
    MeasureType
    FunctionsT1TwoParam<MeasureType>
    ::calcModelValue(const MeasureType* parameters, MeasureType time){

        MeasureType A = parameters[0];
        MeasureType T1 = parameters[1];

        if (fabs(T1) < std::numeric_limits<MeasureType>::min())
            return A;

        return A * (1 - exp( -time / T1 ));
    }

    template< typename MeasureType >
    void
    FunctionsT1TwoParam<MeasureType>
    ::calcLSResiduals(const MeasureType* parameters, MeasureType* residuals){

        unsigned int nSamples = this->_nSamples;

        for (unsigned int i = 0; i < nSamples; i++) {
            MeasureType invTime = this->_InvTimes[i];
            MeasureType measured = this->_Signal[i];
            MeasureType calculated = 0;

            calculated = calcModelValue(parameters, invTime);

            residuals[i] = calculated - measured;
        }
    }

    template< typename MeasureType >
    void
    FunctionsT1TwoParam<MeasureType>
    ::calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian){

        int nSamples = this->_nSamples;

        MeasureType A = parameters[0];
        MeasureType T1 = parameters[1];
        MeasureType invTime, myexp;

        for (int i = 0; i < nSamples; i++) {
            invTime = this->_InvTimes[i];
            myexp = exp(-invTime/T1);

            // calculated in matlab (syms A T1 t), f=A*(1-exp(-t./T1)); diff(f,A),  diff(f,T1)
            jacobian[i*2+0] = 1.0 - myexp;
            jacobian[i*2+1] = -( A * invTime * myexp )/ ( T1 * T1 );
        }
    }

    template< typename MeasureType >
    MeasureType
    FunctionsT1TwoParam<MeasureType>
    ::calcCostValue(const MeasureType* parameters){

        int nSamples = this->_nSamples;

        calcLSResiduals(parameters, this->_Residuals);
        MeasureType result = 0;

        for (int i = 0; i < nSamples; ++i) {
            result = result + this->_Residuals[i] * this->_Residuals[i];
        }

        return result;
    }

    template< typename MeasureType >
    void
    FunctionsT1TwoParam<MeasureType>
    ::calcCostDerivative(const MeasureType* parameters, MeasureType* derivative){

        int nSamples = this->_nSamples;

        derivative[0] = 0;
        derivative[1] = 0;

        MeasureType measured, invTime, myexp;

        MeasureType A = parameters[0];
        MeasureType T1 = parameters[1];

        // calculated in matlab (syms A B T1 t y), f=(A-B*exp(-t./T1)-y).^2; diff(f,A), diff(f,B), diff(f,T1)
        for (int i = 0; i < nSamples; ++i){
            measured = this->getSignal()[i];
            invTime = this->getInvTimes()[i];
            myexp = exp(-invTime / T1);

            derivative[0] = derivative[0] + 2 * (myexp - 1)*(measured + A*(myexp - 1));
            derivative[1] = derivative[1] + ( 2 * A * invTime * myexp * ( measured + A * ( myexp - 1 ))) / ( T1 * T1 );
        }
    }

} //namespace Ox


#endif //Tomato_OXFUNCTIONST1TwoParam_HXX
