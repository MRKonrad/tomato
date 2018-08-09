/*!
 * \file OxFunctionsT1.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef OXSHMOLLI2_OXFUNCTIONST1_H
#define OXSHMOLLI2_OXFUNCTIONST1_H

#include <iostream>

namespace Ox {

    /**
     * \class FunctionsT1
     * \brief Container for a model function, cost function and Least-Squares function. And derivatives.
     * \details Here model function is defined - calcModelValue(). Fitting algorithms based on optimisation need a cost
     * function - calcCostValue(). Fitting algorithms based on least squares need a residuals calculation -
     * calcLSResiduals(). Some fitting algorithms use derivatives, hence calcLSJacobian() and calcCostDerivative().
     * The memeber variables are pointers to c-arrays, we need to know how many samples we want to process. Thats the
     * nSamples defined in the constructor.
     * TODO: think about moving _Parameters out of this class and take it as a parameter of all the above functions
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class FunctionsT1{

    public:

        /**
         * calcModelValue the most important function of this class
         * @param time
         * @return model(time)
         */
        virtual MeasureType calcModelValue(MeasureType time) = 0;

        /**
         * calcLSResiduals the most important function of this class
         * @param residuals
         */
        virtual void calcLSResiduals(MeasureType* residuals) = 0;

        /**
         * calcCostValue the most important function of this class
         * @return
         */
        virtual MeasureType calcCostValue() = 0;

        /**
         * calcCostDerivative the most important function of this class
         * @param derivative
         */
        virtual void calcCostDerivative(MeasureType* derivative) = 0;

        /**
         * calcLSJacobian the most important function of this class
         * @param jacobian - 2d matrix stored as 1d array
         */
        virtual void calcLSJacobian(MeasureType* jacobian) = 0;

        // getters
        virtual const MeasureType *getInvTimes() const { return _InvTimes; }
        virtual const MeasureType *getSatTimes() const { return _SatTimes; }
        virtual const MeasureType *getRepTimes() const { return _RepTimes; }
        virtual const MeasureType *getRelAcqTimes() const { return _RelAcqTimes; }
        virtual const MeasureType *getSignal() const { return _Signal; }
        virtual MeasureType *getParameters() { return _Parameters; }
        virtual int getNSamples() { return _nSamples; }

        // setters
        virtual void setInvTimes(const MeasureType *_InvTimes) { FunctionsT1::_InvTimes = _InvTimes; }
        virtual void setSatTimes(const MeasureType *_SatTimes) { FunctionsT1::_SatTimes = _SatTimes; }
        virtual void setRepTimes(const MeasureType *_RepTimes) { FunctionsT1::_RepTimes = _RepTimes; }
        virtual void setRelAcqTimes(const MeasureType *_RelAcqTimes) { FunctionsT1::_RelAcqTimes = _RelAcqTimes; }
        virtual void setSignal(const MeasureType *_Signal) {FunctionsT1::_Signal = _Signal; }
        virtual void setParameters( MeasureType *_Parameters) { FunctionsT1::_Parameters = _Parameters; }
        void setNSamples(int _nSamples) { FunctionsT1::_nSamples = _nSamples; }

        virtual void copyToParameters(const MeasureType *ptrFrom){
            for (int i = 0; i < _nDims; ++i){
                _Parameters[i] = ptrFrom[i];
            }
        }

        /**
         * \brief show me your FunctionsT1
         */
        virtual void disp(){
            int nSamples = this->getNSamples();
            std::cout << "\nYou called disp() on a FunctionsT1 object " << this << " with nSamples: " << getNSamples();
            KWUtil::printArray((bool)getInvTimes(), nSamples, getInvTimes(),       (char*)"\nInvTimes:    ");
            KWUtil::printArray((bool)getSatTimes(), nSamples, getSatTimes(),       (char*)"\nSatTimes:    ");
            KWUtil::printArray((bool)getRepTimes(), nSamples, getRepTimes(),       (char*)"\nRepTimes:    ");
            KWUtil::printArray((bool)getRelAcqTimes(), nSamples, getRelAcqTimes(), (char*)"\nRelAcqTimes: ");
            KWUtil::printArray((bool)getSignal(), nSamples, getSignal(),           (char*)"\nSignal:      ");
            KWUtil::printArray((bool)getParameters(), _nDims, getParameters(),          (char*)"\nParameters:  ");
            std::cout << std::endl;
        }

        /**
         * \brief set all the pointers to zero
         */
        void init(){
            _InvTimes = 0;
            _SatTimes = 0;
            _RepTimes = 0;
            _RelAcqTimes = 0;
            _Signal = 0;
            _Parameters = 0;
        }

        /**
         * \brief default constructor
         */
        FunctionsT1(){
            //std::cout << "FunctionsT1 constructor" << std::endl;
            _nSamples = 0;
            _nDims = 3;
            init();
        };

        /**
         * \brief copy constructor keeps only  and _nDims
         * @param old
         */
        FunctionsT1(const FunctionsT1 &old) {
            std::cout<<"FunctionsT1 copy constructor "<<std::endl;
            _nSamples = old._nSamples;
            _nDims = old._nDims;
            init();
        }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT1(){};

    protected:

        const MeasureType* _InvTimes;
        const MeasureType* _SatTimes;
        const MeasureType* _RepTimes;
        const MeasureType* _RelAcqTimes;
        const MeasureType* _Signal;
        MeasureType* _Parameters;
        int _nSamples;
        int _nDims = 3;
    };
} //namespace Ox

#endif //OXSHMOLLI2_OXFUNCTIONST1_H
