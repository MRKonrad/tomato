/*!
 * \file OxSignCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXSIGNCALCULATOR_H
#define Tomato_OXSIGNCALCULATOR_H

#include "KWUtil.h"

namespace Ox {

    /**
     * \class SignCalculator
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class SignCalculator{

    public:

        // getters
        virtual const MeasureType *getInvTimes() const {
            if (!_InvTimes) throw std::runtime_error("SignCalculator: _InvTimes equals 0. Set _InvTimes");
            return _InvTimes;
        }

        virtual const MeasureType *getSigMag() const {
            if (!_SigMag) throw std::runtime_error("SignCalculator: _SigMag equals 0. Set _SigMag");
            return _SigMag;
        }

        virtual const MeasureType *getSigPha() const {
            if (!_SigPha) throw std::runtime_error("SignCalculator: _SigPha equals 0. Set _SigPha");
            return _SigPha;
        }

        virtual MeasureType *getSignal() {
            if (!_Signal) throw std::runtime_error("SignCalculator: _SigMag equals 0. Set _SigMag");
            return _Signal;
        }

        virtual MeasureType *getSigns() {
            if (!_Signs) throw std::runtime_error("SignCalculator: _Signs equals 0. Set _Signs");
            return _Signs;
        }

        virtual int getNSamples() {
            if (!_nSamples) throw std::runtime_error("SignCalculator: _nSamples equals 0. Set _nSamples");
            return _nSamples;
        }

        //setters
        virtual void setInvTimes(const MeasureType *_InvTimes) { SignCalculator::_InvTimes = _InvTimes; }
        virtual void setSigMag(const MeasureType *_SigMag) { SignCalculator::_SigMag = _SigMag; }
        virtual void setSigPha(const MeasureType *_SigPha) { SignCalculator::_SigPha = _SigPha; }
        virtual void setSignal(MeasureType *_Signal) { SignCalculator::_Signal = _Signal; }
        virtual void setSigns(MeasureType *_Signs) { SignCalculator::_Signs = _Signs; }
        virtual void setNSamples(int _nSamples) { SignCalculator::_nSamples = _nSamples; }

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateSign() = 0;

        virtual void disp(){
            int nSamples = this->getNSamples();
            std::cout << "\nYou called disp() on a SignCalculator object " << this << " with nSamples: " << getNSamples();
            KWUtil::printArray(_InvTimes !=0, nSamples, _InvTimes, (char*)"\nInvTimes:    ");
            KWUtil::printArray(_SigMag !=0,   nSamples, _SigMag,   (char*)"\nSigMag:      ");
            KWUtil::printArray(_SigPha !=0,   nSamples, _SigPha,   (char*)"\nSigPha:      ");
            KWUtil::printArray(_Signal !=0,   nSamples, _Signal,   (char*)"\nSignal:      ");
            KWUtil::printArray(_Signs !=0,    nSamples, _Signs,    (char*)"\nSigns:       ");
            std::cout << std::endl;
        }

        /**
         * \brief set all the pointers to zero
         */
        void setAllPointersToNull(){
            _InvTimes = 0;
            _SigMag = 0;
            _SigPha = 0;
            _Signal = 0;
            _Signs = 0;
        }

        /**
         * \brief constructor
         */
        SignCalculator(){
            setAllPointersToNull();
            _nSamples = 0;
        };

        /**
         * \brief copy constructor
         */
        SignCalculator(const SignCalculator &old){
            setAllPointersToNull();
            _nSamples = old._nSamples;
        };

        /**
         * cloning
         * @return
         */
        virtual SignCalculator<MeasureType> *newByCloning() = 0; //{ return new SignCalculator<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~SignCalculator(){};

    protected:
        const MeasureType* _InvTimes;
        const MeasureType* _SigMag; // original one
        const MeasureType* _SigPha; // original one
        MeasureType* _Signal; // we will be working with this one
        MeasureType* _Signs;  // we will be working with this one
        int _nSamples;

    };
} //namespace Ox

#endif //Tomato_OXSIGNCALCULATOR_H
