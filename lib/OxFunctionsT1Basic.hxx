/*!
 * \file OxFunctionsT1Basic.hxx
 * \author Konrad Werys
 * \date 2018/07/29
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

        //std::cout << "calcLSResiduals" << std::endl;
        unsigned int nSamples = this->_nSamples;

        MeasureType A = this->_Parameters[0];
        MeasureType B = this->_Parameters[1];
        MeasureType T1star = this->_Parameters[2];

        for (unsigned int i = 0; i < nSamples; i++) {
            MeasureType invTime = this->_InvTimes[i];
            MeasureType measured = this->_Signal[i];
            MeasureType calculated = 0;

            calculated = calcModelValue(invTime);

            residuals[i] = calculated - measured;
        }
    }

    template< typename MeasureType >
    void
    FunctionsT1Basic<MeasureType>
    ::calcLSJacobian(MeasureType* jacobian){
        unsigned int nSamples = this->_nSamples;

        //MeasureType A = this->_Parameters[0];
        MeasureType B = this->_Parameters[1];
        MeasureType T1star = this->_Parameters[2];
        MeasureType invTime, myexp;

        for (int i = 0; i < nSamples; i++) {
            invTime = this->_InvTimes[i];
            myexp = exp(-invTime/T1star);

            // calculated in matlab (syms A B T1 t), f=A-B*exp(-t./T1); diff(f,A), diff(f,B), diff(calcCostValue,T1)
            jacobian[i*3+0] = 1.0;
            jacobian[i*3+1] = -myexp;
            jacobian[i*3+2] = -B*invTime*myexp/(T1star*T1star);
        }
    }

    template< typename MeasureType >
    MeasureType
    FunctionsT1Basic<MeasureType>
    ::calcCostValue(){
        //std::cout << "calcCostValue" << std::endl;
        int nSamples = this->_nSamples;

        MeasureType *residuals = new MeasureType[nSamples];
        calcLSResiduals(residuals);
        MeasureType result = 0;

        for (int i = 0; i < nSamples; ++i) {
            result = result + residuals[i] * residuals[i];
        }

        delete [] residuals;
        return result;
    }

    template< typename MeasureType >
    void
    FunctionsT1Basic<MeasureType>
    ::calcCostDerivative(MeasureType* derivative){
        //std::cout << "calcCostDerivative" << std::endl;

        unsigned int nSamples = this->_nSamples;

        derivative[0] = 0;
        derivative[1] = 0;
        derivative[2] = 0;

        MeasureType measured, invTime, myexp;

        MeasureType A = this->_Parameters[0];
        MeasureType B = this->_Parameters[1];
        MeasureType T1star = this->_Parameters[2];

        // calculated in matlab (syms A B T1 t y), f=(A-B*exp(-t./T1)-y).^2; diff(f,A), diff(f,B), diff(calcCostValue,T1)
        for (int i = 0; i < nSamples; ++i){
            measured = this->getSignal()[i];
            invTime = this->getInvTimes()[i];
            myexp = exp(-invTime/T1star);

            derivative[0] = derivative[0] + A*2 - measured*2 - myexp*B*2;;
            derivative[1] = derivative[1] + myexp*2*(measured - A + myexp*B);
            derivative[2] = derivative[2] + (invTime*myexp*B*2*(measured - A + myexp*B))/(T1star*T1star);
        }
    }

} //namespace Ox


#endif //OXSHMOLLI2_OXFUNCTIONST1BASIC_HXX
