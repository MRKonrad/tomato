/*!
 * \file OxModelT1Shmolli.hxx
 * \author Konrad Werys
 * \date 2018/08/22
 */

#ifndef Tomato_OXModelT1Shmolli_HXX
#define Tomato_OXModelT1Shmolli_HXX

namespace Ox {


    template< typename MeasureType >
    MeasureType
    ModelT1Shmolli<MeasureType>
    ::calcModelValue(const MeasureType* parameters, MeasureType time){
        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T1star = parameters[2];

        MeasureType calculated = 0;

        if (_preventUnderOverFlow){
            if (fabs(T1star) <= (time*0.005 ))
                calculated = A; //prevent underflow when (-mTI/A[2]) < (-500) -- div/0, e^-500 is 0.0000
            else if (fabs(T1star) >= (time*500 ))
                calculated = (A - B*exp((MeasureType)-500)); //prevent overflow
        }

        // if the previous block has calculated someting
        if (calculated != 0){
            return calculated;
        }

        // if the numbers are unreasonable
        if (fabs(T1star) < std::numeric_limits<MeasureType>::min()){
            return calculated;
        }

        if (_expAbsCost) {
            calculated = A - B * exp((MeasureType) -fabs( time / T1star ) );
        } else {
            calculated = A - B * exp((MeasureType) -time / T1star );
        }

        return calculated;
    }

    template< typename MeasureType >
    void
    ModelT1Shmolli<MeasureType>
    ::calcLSResiduals(const MeasureType* parameters, MeasureType* residuals){

        //std::cout << "calcLSResiduals" << std::endl;
        unsigned int nSamples = this->_nSamples;

        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T1star = parameters[2];

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
    ModelT1Shmolli<MeasureType>
    ::calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian){
        int nSamples = this->_nSamples;

        //MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T1star = parameters[2];
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
    ModelT1Shmolli<MeasureType>
    ::calcCostValue(const MeasureType* parameters){

        //std::cout << "calcCostValue" << std::endl;
        int nSamples = this->_nSamples;

        calcLSResiduals(parameters, this->_Residuals);
        MeasureType result = 0;

        if (_rootMedianSquareCost){
            // residuals are nor residuals squared!!!
            for (int i = 0; i < nSamples; ++i) {
                this->_Residuals[i] = this->_Residuals[i] * this->_Residuals[i];
            }
            // residuals are nor residuals squared!!!
            result = KWUtil::calcMedianArray(nSamples, this->_Residuals);
            // residuals are nor residuals squared!!!

        } else {

            for (int i = 0; i < nSamples; ++i) {
                result = result + this->_Residuals[i] * this->_Residuals[i];
            }

        }

        //// SKP magic
        if (_costHeuristic){
            if(parameters[0] < 0.  )
                result -= parameters[0]; // A<0 negative Mz0 estimate
            if(parameters[0] > parameters[1])
                result += (parameters[0] - parameters[1]); // less than saturation: B<A, should be B=2*A ideally
            if(parameters[1] > (4. * parameters[0]))
                result += (parameters[1]-4. * parameters[0]); // inversion ratio very high, likely error or SFFP
            if(parameters[2] < 0.)
                result -= parameters[2]; // negative time constant
        }

        return result;


    }

    template< typename MeasureType >
    void
    ModelT1Shmolli<MeasureType>
    ::calcCostDerivative(const MeasureType* parameters, MeasureType* derivative){
        //std::cout << "calcCostDerivative" << std::endl;

        int nSamples = this->_nSamples;

        derivative[0] = 0;
        derivative[1] = 0;
        derivative[2] = 0;

        MeasureType measured, invTime, myexp;

        MeasureType A = parameters[0];
        MeasureType B = parameters[1];
        MeasureType T1star = parameters[2];

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


#endif //Tomato_OXModelT1Shmolli_HXX
