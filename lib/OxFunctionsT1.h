/*!
 * \file OxFunctionsT1.h
 * \author Konrad Werys
 * \date 2018/08/29
 */

#ifndef OXSHMOLLI2_OXFUNCTIONST1_H
#define OXSHMOLLI2_OXFUNCTIONST1_H

namespace Ox {

    /**
     * \class FunctionsT1
     * \brief Container for a model function, cost function and Least-Squares function. And derivatives.
     * \details Here model function is defined - calcModelValue(). Fitting algorithms based on optimisation need a cost
     * function - calcCostValue(). Fitting algorithms based on least squares need a residuals calculation -
     * calcLSResiduals(). Some fitting algorithms use derivatives, hence calcLSJacobian() and calcCostDerivative().
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT1{

    public:

        virtual MeasureType calcModelValue(MeasureType time) = 0;
        virtual void calcLSResiduals(MeasureType* residuals) = 0;
        virtual MeasureType calcCostValue() = 0;
        virtual void calcCostDerivative(MeasureType* derivative) = 0;
        virtual void calcLSJacobian(MeasureType** jacobian) = 0;

        virtual MeasureType *getInvTimes() const { return _InvTimes; }
        virtual MeasureType *getSatTimes() const { return _SatTimes; }
        virtual MeasureType *getRepTimes() const { return _RepTimes; }
        virtual MeasureType *getRelAcqTimes() const { return _RelAcqTimes; }
        virtual MeasureType *getSignal() const { return _Signal; }
        virtual MeasureType *getParameters() const { return _Parameters; }

        virtual void setInvTimes(MeasureType *_InvTimes) { FunctionsT1::_InvTimes = _InvTimes; }
        virtual void setSatTimes(MeasureType *_SatTimes) { FunctionsT1::_SatTimes = _SatTimes; }
        virtual void setRepTimes(MeasureType *_RepTimes) { FunctionsT1::_RepTimes = _RepTimes; }
        virtual void setRelAcqTimes(MeasureType *_RelAcqTimes) { FunctionsT1::_RelAcqTimes = _RelAcqTimes; }
        virtual void setSignal(MeasureType *_Signal) { FunctionsT1::_Signal = _Signal; }
        virtual void setParameters(MeasureType *_Parameters) { FunctionsT1::_Parameters = _Parameters; }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        ~FunctionsT1(){};

    protected:
        MeasureType* _InvTimes;
        MeasureType* _SatTimes;
        MeasureType* _RepTimes;
        MeasureType* _RelAcqTimes;
        MeasureType* _Signal;
        MeasureType* _Parameters;
    };
} //namespace Ox

#endif //OXSHMOLLI2_OXFUNCTIONST1_H
