/*!
 * \file OxSignCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef OXSHMOLLI2_OXSIGNCALCULATOR_H
#define OXSHMOLLI2_OXSIGNCALCULATOR_H

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
            if (!_InvTimes) { std::cerr << "_InvTimes equals 0. Set _InvTimes" << std::endl; throw std::exception(); }
            return _InvTimes;
        }

        virtual const MeasureType *getSigMag() const {
            if (!_SigMag) { std::cerr << "_SigMag equals 0. Set _SigMag" << std::endl; throw std::exception(); }
            return _SigMag;
        }

        virtual const MeasureType *getSigPha() const {
            if (!_SigPha) { std::cerr << "_SigPha equals 0. Set _SigPha" << std::endl; throw std::exception(); }
            return _SigPha;
        }

        virtual MeasureType *getSignal() {
            if (!_Signal) { std::cerr << "_Signal equals 0. Set _Signal" << std::endl; throw std::exception(); }
            return _Signal;
        }

        virtual MeasureType *getSigns() {
            if (!_Signs) { std::cerr << "_Signs equals 0. Set _Signs" << std::endl; throw std::exception(); }
            return _Signs;
        }

        virtual int getNSamples() {
            if (!_nSamples) { std::cerr << "_nSamples equals 0. Set _nSamples" << std::endl; throw std::exception(); }
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
        virtual int calculateSign(){

            for (int i = 0; i < getNSamples(); ++i) {
                getSignal()[i] = getSigMag()[i];
                getSigns() [i] = 0;
            }

            return 0; // EXIT_SUCCESS
        }

        /**
         * \brief default constructor
         */
        SignCalculator(){
            _InvTimes = 0;
            _SigMag = 0;
            _SigPha = 0;
            _Signal = 0;
            _Signs = 0;
            _nSamples = 0;
        };

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

#endif //OXSHMOLLI2_OXSIGNCALCULATOR_H
