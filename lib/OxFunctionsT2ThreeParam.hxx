/*!
 * \file OxFunctionsT2ThreeParam.hxx
 * \author Konrad Werys
 * \date 2019/11/04
 */

#ifndef Tomato_OXFUNCTIONST2ThreeParam_HXX
#define Tomato_OXFUNCTIONST2ThreeParam_HXX

namespace Ox {


    template< typename MeasureType >
    MeasureType
    FunctionsT2ThreeParam<MeasureType>
    ::calcModelValue(const MeasureType* parameters, MeasureType time){
        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T2 = parameters[2];

        if (fabs(T2) < std::numeric_limits<MeasureType>::min())
            return A;

        return A + B * exp( -time / T2 );
    }

    template< typename MeasureType >
    void
    FunctionsT2ThreeParam<MeasureType>
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
    FunctionsT2ThreeParam<MeasureType>
    ::calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian){
        int nSamples = this->_nSamples;

        //MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T2 = parameters[2];
        MeasureType echoTime, myexp;

        for (int i = 0; i < nSamples; i++) {
            echoTime = this->_EchoTimes[i];
            myexp = exp(-echoTime / T2);

            // calculated in matlab
            // syms A B T2 t
            // f=A+B*exp(-t./T2); diff(f,A), diff(f,B), diff(f,T2)
            jacobian[i*3+0] = 1.0;
            jacobian[i*3+1] = myexp;
            jacobian[i*3+2] = B * echoTime * myexp / (T2 * T2);
        }
    }

    template< typename MeasureType >
    MeasureType
    FunctionsT2ThreeParam<MeasureType>
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
    FunctionsT2ThreeParam<MeasureType>
    ::calcCostDerivative(const MeasureType* parameters, MeasureType* derivative){

        int nSamples = this->_nSamples;

        derivative[0] = 0;
        derivative[1] = 0;
        derivative[2] = 0;

        MeasureType measured, echoTime, myexp;

        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T2 = parameters[2];

        // calculated in matlab
        // syms A B T2 t y
        // f=(A+B*exp(-t./T2)-y).^2; diff(f,A), diff(f,B), diff(f,T2)
        for (int i = 0; i < nSamples; ++i){
            measured = this->getSignal()[i];
            echoTime = this->getEchoTimes()[i];
            myexp = exp(-echoTime / T2);

            derivative[0] = derivative[0] + A*2 - measured*2 + myexp*B*2;;
            derivative[1] = derivative[1] + myexp*2*(-measured + A + myexp*B);
            derivative[2] = derivative[2] + (echoTime * myexp * B * 2 * (-measured + A + myexp * B)) / (T2 * T2);
        }
    }

} //namespace Ox


#endif //Tomato_OXFUNCTIONST2ThreeParam_HXX
