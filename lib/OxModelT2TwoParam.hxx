/*!
 * \file OxModelT2TwoParam.hxx
 * \author Konrad Werys
 * \date 2019/11/14
 */

#ifndef Tomato_OXModelT2TwoParam_HXX
#define Tomato_OXModelT2TwoParam_HXX

namespace Ox {


    template< typename MeasureType >
    MeasureType
    ModelT2TwoParam<MeasureType>
    ::calcModelValue(const MeasureType* parameters, MeasureType time){
        MeasureType A = parameters[0];
        MeasureType T2 = parameters[1];

        if (fabs(T2) < std::numeric_limits<MeasureType>::min())
            return A;

        return A * exp( -time / T2 );
    }

    template< typename MeasureType >
    void
    ModelT2TwoParam<MeasureType>
    ::calcLSResiduals(const MeasureType* parameters, MeasureType* residuals){

        unsigned int nSamples = this->_nSamples;

        for (unsigned int i = 0; i < nSamples; i++) {
            MeasureType invTime = this->_EchoTimes[i];
            MeasureType measured = this->_Signal[i];
            MeasureType calculated = 0;

            calculated = calcModelValue(parameters, invTime);

            residuals[i] = calculated - measured;
        }
    }

    template< typename MeasureType >
    void
    ModelT2TwoParam<MeasureType>
    ::calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian){

        int nSamples = this->_nSamples;

        MeasureType A = parameters[0];
        MeasureType T2 = parameters[1];
        MeasureType echoTime, myexp;

        for (int i = 0; i < nSamples; i++) {
            echoTime = this->_EchoTimes[i];
            myexp = exp(-echoTime / T2);

            // calculated in matlab
            // syms A T2 t
            // f=A*exp(-t./T2); diff(f,A), diff(f,T2)
            jacobian[i*2+0] = myexp;
            jacobian[i*2+1] = A * echoTime * myexp / T2 / T2;
        }
    }

    template< typename MeasureType >
    MeasureType
    ModelT2TwoParam<MeasureType>
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
    ModelT2TwoParam<MeasureType>
    ::calcCostDerivative(const MeasureType* parameters, MeasureType* derivative){

        int nSamples = this->_nSamples;

        derivative[0] = 0;
        derivative[1] = 0;

        MeasureType measured, echoTime, myexp;

        MeasureType A = parameters[0];
        MeasureType T2 = parameters[1];

        // calculated in matlab
        // syms A T2 t y
        // f=(A+exp(-t./T2)-y).^2; diff(f,A), diff(f,T2)
        for (int i = 0; i < nSamples; ++i){
            measured = this->getSignal()[i];
            echoTime = this->getEchoTimes()[i];
            myexp = exp(-echoTime / T2);

            derivative[0] = derivative[0] + A*2 - measured*2 + myexp*2;;
            derivative[1] = derivative[1] + 2 * echoTime * myexp * (A - measured + myexp) / T2 / T2;
        }
    }

} //namespace Ox


#endif //Tomato_OXModelT2TwoParam_HXX
