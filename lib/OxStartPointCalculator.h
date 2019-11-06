/*!
 * \file OxStartPointCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXSTARTPOINTCALCULATOR_H
#define Tomato_OXSTARTPOINTCALCULATOR_H

#include "KWUtil.h"

namespace Ox {

    /**
     * \class StartPointCalculator
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class StartPointCalculator{

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateStartPoint() = 0;

        // getters
        const MeasureType *getInputStartPoint() const {
            return _InputStartPoint;
        }
        MeasureType *getCalculatedStartPoint() const {
            if (!_CalculatedStartPoint) throw std::runtime_error("_CalculatedStartPoint equals 0. Set _CalculatedStartPoint");
            return _CalculatedStartPoint;
        }

        int getNDims() const { return _nDims; }

        //setters
        virtual void setNDims(int nDims) { _nDims = nDims; }
        virtual void setInvTimes(const MeasureType *_InvTimes) { StartPointCalculator::_InvTimes = _InvTimes; }
        virtual void setEchoTimes(const MeasureType *_EchoTimes) { StartPointCalculator::_EchoTimes = _EchoTimes; }
        virtual void setSigMag(const MeasureType *_SigMag) { StartPointCalculator::_SigMag = _SigMag; }
        virtual void setSigns(const MeasureType *_Signs) { StartPointCalculator::_Signs = _Signs; }
        virtual void setInputStartPoint(const MeasureType *_InputStartPoint) { StartPointCalculator::_InputStartPoint = _InputStartPoint; }
        virtual void setCalculatedStartPoint(MeasureType *_CalculatedStartPoint) { StartPointCalculator::_CalculatedStartPoint = _CalculatedStartPoint; }
        virtual void setNSamples(int _nSamples) { StartPointCalculator::_nSamples = _nSamples; }


        void disp(){
            std::cout << "\nYou called disp() on a StartPointCalculator object " << this
                      << " with nSamples: " << _nSamples
                      << " nDims: " << _nDims;
            KWUtil::printArray((bool)_InvTimes, _nSamples, _InvTimes,       (char*)"\nInvTimes:    ");
            KWUtil::printArray((bool)_EchoTimes, _nSamples, _InvTimes,      (char*)"\nEchoTimes:    ");
            KWUtil::printArray((bool)_SigMag, _nSamples, _SigMag,           (char*)"\nSigMag:      ");
            KWUtil::printArray((bool)_Signs, _nSamples, _Signs,             (char*)"\nSigns:       ");
            KWUtil::printArray(_nDims, this->getCalculatedStartPoint(),     (char*)"\nStart point: ");
        }

        /**
         * \brief set all the pointers to zero
         */
        void setAllPointersToNull(){
            _InputStartPoint = 0;
            _CalculatedStartPoint = 0;
            _InvTimes = 0;
            _EchoTimes = 0;
            _SigMag = 0;
            _Signs = 0;
        }

        /**
          * \brief constructor
          */
        StartPointCalculator(){

            setAllPointersToNull();

            _nSamples = 0;
            _nDims = 0;
        }

        /**
         * \brief copy constructor
         */
        StartPointCalculator(const StartPointCalculator &old){

            setAllPointersToNull();

            _nSamples = old._nSamples;
            _nDims = old._nDims;
        };

        /**
         * cloning
         * @return
         */
        virtual StartPointCalculator<MeasureType> *newByCloning() = 0;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculator(){};

    protected:
        const MeasureType* _InputStartPoint;
        MeasureType* _CalculatedStartPoint;

        const MeasureType* _InvTimes;
        const MeasureType* _EchoTimes;
        const MeasureType* _SigMag;
        const MeasureType* _Signs;

        int _nSamples;
        int _nDims; // special ShMOLLI parameter

    };
} //namespace Ox

#endif //Tomato_OXSTARTPOINTCALCULATOR_H
