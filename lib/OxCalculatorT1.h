/*!
 * \file OxCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef OXSHMOLLI2_OXCALCULATORT1_H
#define OXSHMOLLI2_OXCALCULATORT1_H

#include "OxFitter.h"
#include "OxFunctionsT1.h"
#include "OxSignCalculator.h"
#include "OxStartPointCalculator.h"

namespace Ox {

    /**
     * \class CalculatorT1
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class CalculatorT1{

    public:

        // getters
        FunctionsT1<MeasureType> *getFunctionsT1() const {
            if (!_FunctionsT1) {
                std::cerr << "_FunctionsT1 equals 0. Set _FunctionsT1" << std::endl;
                throw std::exception();
            };
            return _FunctionsT1;
        }

        Fitter<MeasureType> *getFitter() const {
            if (!_Fitter) {
                std::cerr << "_Fitter equals 0. Set _Fitter" << std::endl;
                throw std::exception();
            };
            return _Fitter;
        }

        StartPointCalculator<MeasureType> *getStartPointCalculator() const {
            if (!_StartPointCalculator) {
                std::cerr << "_StartPointCalculator equals 0. Set _StartPointCalculator" << std::endl;
                throw std::exception();
            };
            return _StartPointCalculator;
        }

        SignCalculator<MeasureType> *getSignCalculator() const {
            if (!_SignCalculator) {
                std::cerr << "_SignCalculator equals 0. Set _SignCalculator" << std::endl;
                throw std::exception();
            };
            return _SignCalculator;
        }

        const MeasureType *getInvTimes() const {
            if (!_InvTimes) {
                std::cerr << "_InvTimes equals 0. Set _InvTimes" << std::endl;
                throw std::exception();
            };
            return _InvTimes;
        }

        const MeasureType *getSigMag() const {
            if (!_SigMag) {
                std::cerr << "_SigMag equals 0. Set _SigMag" << std::endl;
                throw std::exception();
            };
            return _SigMag;
        }

        const MeasureType *getSigPha() const {
            if (!_SigPha) {
                std::cerr << "_SigPha equals 0. Set _SigPha" << std::endl;
                throw std::exception();
            };
            return _SigPha;
        }

        const MeasureType *getResults() const { return _Results; }

        int getNSamples() const {
            if (!_nSamples) {
                std::cerr << "_nSamples equals 0. Set _nSamples" << std::endl;
                throw std::exception();
            };
            return _nSamples;
        }

        // setters for the 'has a' classes
        void setFunctionsT1(FunctionsT1<MeasureType> *_FunctionsT1) { CalculatorT1::_FunctionsT1 = _FunctionsT1; }
        void setFitter(Fitter<MeasureType> *_Fitter) { CalculatorT1::_Fitter = _Fitter; }
        void setSignCalculator(SignCalculator<MeasureType> *_SignCalculator) { CalculatorT1::_SignCalculator = _SignCalculator; }
        void setStartPointCalculator(StartPointCalculator<MeasureType> *_StartPointCalculator) { CalculatorT1::_StartPointCalculator = _StartPointCalculator; }

        // setters for the data
        virtual void setInvTimes(const MeasureType *_InvTimes) { CalculatorT1::_InvTimes = _InvTimes; }
        virtual void setSigMag(const MeasureType *_SigMag) { CalculatorT1::_SigMag = _SigMag; }
        virtual void setSigPha(const MeasureType *_SigPha) { CalculatorT1::_SigPha = _SigPha; }
        //virtual void setSignal(MeasureType *_Signal) { CalculatorT1::_Signal = _Signal; }
        //virtual void setSigns(MeasureType *_Signs) { CalculatorT1::_Signs = _Signs; }
        //virtual void setParameters(MeasureType *_Parameters) { CalculatorT1::_Parameters = _Parameters; }

        virtual void setNSamples(int _nSamples) {

            delete [] _Signal; _Signal = 0;
            delete [] _Signs; _Signs = 0;

            CalculatorT1::_nSamples = _nSamples;

            _Signal = new MeasureType[_nSamples];
            _Signs = new MeasureType[_nSamples];
        }

        /**
         * do all the checks and  prepare to do the calculation, for example calc signal/signs and _TRRaverageHB
         * @return success/failure
         */
        virtual int prepareToCalculate(){

            getSignCalculator()->setNSamples(getNSamples());
            getSignCalculator()->setInvTimes(getInvTimes());
            getSignCalculator()->setSigMag(getSigMag());
            getSignCalculator()->setSigPha(getSigPha());
            getSignCalculator()->setSignal(_Signal);
            getSignCalculator()->setSigns(_Signs);

            getSignCalculator()->calculateSign();

            getStartPointCalculator()->setCalculatedStartPoint(_StartPoint);
            getStartPointCalculator()->calculateStartPoint();

            return 0; // EXIT_SUCCESS
        };

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculate() = 0;

        /**
         * \brief default constructor
         */

        CalculatorT1(){
            _FunctionsT1 = 0;
            _Fitter = 0;
            _SignCalculator = 0;
            _StartPointCalculator = 0;
            _InvTimes = 0;
            _SatTimes = 0;
            _RepTimes = 0;
            _RelAcqTimes = 0;
            _SigMag = 0; // original one
            _SigPha = 0; // original one
            _Signal = 0; // we will be working with this one
            _Signs = 0;  // we will be working with this one
            //_Parameters = 0;
            _TRRaverageHB = 0;
            _MeanCutOff = 0;
            _nSamples = 0;
        };

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~CalculatorT1(){
            std::cout << "in ~CalculatorT1" << std::endl;
            delete [] _Signal; _Signal = 0;
            delete [] _Signs; _Signs = 0;
        };

    protected:
        FunctionsT1<MeasureType>* _FunctionsT1;
        Fitter<MeasureType>* _Fitter;
        SignCalculator<MeasureType>* _SignCalculator;
        StartPointCalculator<MeasureType>* _StartPointCalculator;

        const MeasureType* _InvTimes;
        const MeasureType* _SatTimes;
        const MeasureType* _RepTimes;
        const MeasureType* _RelAcqTimes;
        const MeasureType* _SigMag; // original one
        const MeasureType* _SigPha; // original one
        MeasureType* _Signal; // we will be working with this one
        MeasureType* _Signs;  // we will be working with this one
        MeasureType _StartPoint[3];
        MeasureType _Results[3];
        //MeasureType* _Parameters;
        int _nSamples;
        MeasureType _TRRaverageHB;
        MeasureType _MeanCutOff;

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXCALCULATORT1_H
