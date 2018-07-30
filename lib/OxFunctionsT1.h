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
     * The memeber variables are pointers to c-arrays, we need to know how many samples we want to process. Thats the
     * nSamples defined in the constructor.
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

        virtual const MeasureType *getInvTimes() const { return _InvTimes; }
        virtual const MeasureType *getSatTimes() const { return _SatTimes; }
        virtual const MeasureType *getRepTimes() const { return _RepTimes; }
        virtual const MeasureType *getRelAcqTimes() const { return _RelAcqTimes; }
        virtual const MeasureType *getSignal() const { return _Signal; }
        virtual const MeasureType *getParameters() const { return _Parameters; }

        virtual void setInvTimes(const MeasureType *_InvTimes) { FunctionsT1::_InvTimes = _InvTimes; }
        virtual void setSatTimes(const MeasureType *_SatTimes) { FunctionsT1::_SatTimes = _SatTimes; }
        virtual void setRepTimes(const MeasureType *_RepTimes) { FunctionsT1::_RepTimes = _RepTimes; }
        virtual void setRelAcqTimes(const MeasureType *_RelAcqTimes) { FunctionsT1::_RelAcqTimes = _RelAcqTimes; }
        virtual void setSignal(const MeasureType *_Signal) {FunctionsT1::_Signal = _Signal; }
        virtual void setParameters(const MeasureType *_Parameters) { FunctionsT1::_Parameters = _Parameters; }

        /**
         * \brief The only right constructor is the one that is defining the number of samples that are processed in
         * all the member variables.
         * @param _nSamples
         */
        FunctionsT1(int _nSamples){
            this->_nSamples = _nSamples;
        };

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT1(){};

    protected:

        /**
         * \brief We do not want the default constructor to be called. Ever.
         */
        FunctionsT1(){};

        int _nSamples;
        const MeasureType* _InvTimes;
        const MeasureType* _SatTimes;
        const MeasureType* _RepTimes;
        const MeasureType* _RelAcqTimes;
        const MeasureType* _Signal;
        const MeasureType* _Parameters;
    };
} //namespace Ox

#endif //OXSHMOLLI2_OXFUNCTIONST1_H
