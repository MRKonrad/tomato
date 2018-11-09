/*!
 * \file OxFunctionsT1.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXFUNCTIONST1_H
#define Tomato_OXFUNCTIONST1_H

#include "KWUtil.h"
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
        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time) = 0;

        /**
         * calcLSResiduals the most important function of this class
         * @param residuals
         */
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals) = 0;

        /**
         * calcCostValue the most important function of this class
         * @return
         */
        virtual MeasureType calcCostValue(const MeasureType* parameters) = 0;

        /**
         * calcCostDerivative the most important function of this class
         * @param derivative
         */
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative) = 0;

        /**
         * calcLSJacobian the most important function of this class
         * @param jacobian - 2d matrix stored as 1d array
         */
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian) = 0;

        // getters
        virtual int getNSamples() { return _nSamples; }
        virtual const MeasureType *getInvTimes() const { return _InvTimes; }
        virtual const MeasureType *getEchoTimes() const { return _EchoTimes; }
        virtual const MeasureType *getRepTimes() const { return _RepTimes; }
        virtual const MeasureType *getRelAcqTimes() const { return _RelAcqTimes; }
        virtual const MeasureType *getSignal() const { return _Signal; }
        virtual int getNDims() { return _nDims; }

        // setters
        void setNSamples(int _nSamples) { FunctionsT1::_nSamples = _nSamples; }
        virtual void setInvTimes(const MeasureType *_InvTimes) { FunctionsT1::_InvTimes = _InvTimes; }
        virtual void setEchoTimes(const MeasureType *_EchoTimes) { FunctionsT1::_EchoTimes = _EchoTimes; }
        virtual void setRepTimes(const MeasureType *_RepTimes) { FunctionsT1::_RepTimes = _RepTimes; }
        virtual void setRelAcqTimes(const MeasureType *_RelAcqTimes) { FunctionsT1::_RelAcqTimes = _RelAcqTimes; }
        virtual void setSignal(const MeasureType *_Signal) {FunctionsT1::_Signal = _Signal; }

        /**
         * \brief show me your FunctionsT1
         */
        virtual void disp(){
            int nSamples = this->getNSamples();
            std::cout << "\nYou called disp() on a FunctionsT1 object " << this << " with nSamples: " << getNSamples();
            KWUtil::printArray((bool)_InvTimes, nSamples, _InvTimes,       (char*)"\nInvTimes:    ");
            KWUtil::printArray((bool)_EchoTimes, nSamples, _EchoTimes,       (char*)"\nEchoTimes:    ");
            KWUtil::printArray((bool)_RepTimes, nSamples, _RepTimes,       (char*)"\nRepTimes:    ");
            KWUtil::printArray((bool)_RelAcqTimes, nSamples, _RelAcqTimes, (char*)"\nRelAcqTimes: ");
            KWUtil::printArray((bool)_Signal, nSamples, _Signal,           (char*)"\nSignal:      ");
            // KWUtil::printArray((bool)_Parameters, _nDims, _Parameters,     (char*)"\nParameters:  ");
            std::cout << std::endl;
        }

        /**
         * \brief set all the pointers to zero
         */
        void setAllPointersToNull(){
            _InvTimes = 0;
            _EchoTimes = 0;
            _RepTimes = 0;
            _RelAcqTimes = 0;
            _Signal = 0;
            //_Parameters = 0;
        }

        /**
         * \brief constructor
         */
        FunctionsT1(){
            //std::cout << "FunctionsT1 constructor" << std::endl;

            _nSamples = 0;
            _nDims = 0;

            setAllPointersToNull();
        };

        /**
         * \brief copy constructor keeps only _nSamples and _nDims
         * @param old
         */
        FunctionsT1(const FunctionsT1 &old) {
            //std::cout<<"FunctionsT1 copy constructor "<<std::endl;

            _nSamples = old._nSamples;
            _nDims = old._nDims;

            setAllPointersToNull();
        }

        /**
         * cloning
         * @return
         */
        virtual FunctionsT1<MeasureType> *newByCloning() = 0;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~FunctionsT1(){};

    protected:

        const MeasureType* _InvTimes;
        const MeasureType* _EchoTimes;
        const MeasureType* _RepTimes;
        const MeasureType* _RelAcqTimes;
        const MeasureType* _Signal;
        //MeasureType* _Parameters;
        int _nSamples;
        int _nDims;
    };
} //namespace Ox

#endif //Tomato_OXFUNCTIONST1_H
