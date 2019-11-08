/*!
 * \file OxCalculator.hxx
 * \author Konrad Werys
 * \date 2018/08/09
 */

#ifndef Tomato_OXCALCULATORT1_HXX
#define Tomato_OXCALCULATORT1_HXX

#include "OxCalculator.h"

namespace Ox {

    /* ****************** */
    /* ***  GETTERS   *** */
    /* ****************** */

    template<typename MeasureType>
    std::map<std::string, MeasureType>
    Calculator<MeasureType>
    ::getResults() const {
        return _Results;
    }

    template< typename MeasureType >
    Model<MeasureType>*
    Calculator<MeasureType>
    ::getModel() const {
        if (!_Model) {
            throw std::runtime_error("_Model equals 0. Set _Model");
        };
        return _Model;
    }

    template< typename MeasureType >
    Fitter<MeasureType> *
    Calculator<MeasureType>
    ::getFitter() const {
        if (!_Fitter) {
            throw std::runtime_error("_Fitter equals 0. Set _Fitter");
        };
        return _Fitter;
    }

    template< typename MeasureType >
    StartPointCalculator<MeasureType> *
    Calculator<MeasureType>
    ::getStartPointCalculator() const {
//        if (!_StartPointCalculator) {
//            throw std::runtime_error("_StartPointCalculator equals 0. Set _StartPointCalculator");
//        };
        return _StartPointCalculator;
    }

    template< typename MeasureType >
    SignCalculator<MeasureType> *
    Calculator<MeasureType>
    ::getSignCalculator() const {
//        if (!_SignCalculator) {
//            throw std::runtime_error("_SignCalculator equals 0. Set _SignCalculator");
//        };
        return _SignCalculator;
    }

    template< typename MeasureType >
    const MeasureType *
    Calculator<MeasureType>
    ::getInvTimes() const {
        if (!_InvTimes) {
            throw std::runtime_error("_InvTimes equals 0. Set _InvTimes");
        };
        return _InvTimes;
    }

    template< typename MeasureType >
    const MeasureType *
    Calculator<MeasureType>
    ::getEchoTimes() const {
//        if (!_EchoTimes) {
//            throw std::runtime_error("_EchoTimes equals 0. Set _EchoTimes");
//        };
        return _EchoTimes;
    }

    template< typename MeasureType >
    const MeasureType *
    Calculator<MeasureType>
    ::getRepTimes() const {
        return _RepTimes;
    }

    template< typename MeasureType >
    const MeasureType *
    Calculator<MeasureType>
    ::getRelAcqTimes() const {
        return _RelAcqTimes;
    }

    template< typename MeasureType >
    const MeasureType *
    Calculator<MeasureType>
    ::getSigMag() const {
        if (!_SigMag) {
            throw std::runtime_error("_SigMag equals 0. Set _SigMag" );
        };
        return _SigMag;
    }

    template< typename MeasureType >
    const MeasureType *
    Calculator<MeasureType>
    ::getSigPha() const {
        return _SigPha;
    }

    template< typename MeasureType >
    MeasureType *
    Calculator<MeasureType>
    ::getSignal() const {
        return _Signal;
    }

    template< typename MeasureType >
    MeasureType *
    Calculator<MeasureType>
    ::getSigns() const {
        return _Signs;
    }

    template< typename MeasureType >
    MeasureType *
    Calculator<MeasureType>
    ::getStartPoint()  {
        return _StartPoint;
    }


    template< typename MeasureType >
    MeasureType
    Calculator<MeasureType>
    ::getMeanCutOff() const {
        return _MeanCutOff;
    }

    template< typename MeasureType >
    int
    Calculator<MeasureType>
    ::getNSamples() const {
        if (!_nSamples) {
            throw std::runtime_error("_nSamples equals 0. Set _nSamples");
        };
        return _nSamples;
    }

    template< typename MeasureType >
    int
    Calculator<MeasureType>
    ::getNDims() const {
        if (!_nDims) {
            throw std::runtime_error("_nDims equals 0. Set _nDims");
        };
        return _nDims;
    }

    /* ****************** */
    /* ***  SETTERS   *** */
    /* ****************** */

    // setters for the 'has a' classes
    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setModel(Model<MeasureType> *_Model) {
        setNDims(_Model->getNDims());
        Calculator::_Model = _Model;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setFitter(Fitter<MeasureType> *_Fitter) {
        Calculator::_Fitter = _Fitter;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setSignCalculator(SignCalculator<MeasureType> *_SignCalculator) {
        Calculator::_SignCalculator = _SignCalculator;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setStartPointCalculator(StartPointCalculator<MeasureType> *_StartPointCalculator) {
        Calculator::_StartPointCalculator = _StartPointCalculator;
    }

    // setters for the data
    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setInvTimes(const MeasureType *_InvTimes) {
        Calculator::_InvTimes = _InvTimes;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setEchoTimes(const MeasureType *_EchoTimes) {
        Calculator::_EchoTimes = _EchoTimes;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setSigMag(const MeasureType *_SigMag) {
        Calculator::_SigMag = _SigMag;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setSigPha(const MeasureType *_SigPha) {
        Calculator::_SigPha = _SigPha;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setMeanCutOff(MeasureType _MeanCutOff) {
        Calculator::_MeanCutOff = _MeanCutOff;
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setNSamples(int _nSamples) {

        delete [] _Signal; _Signal = 0;
        delete [] _Signs; _Signs = 0;

        Calculator::_nSamples = _nSamples;

        _Signal = new MeasureType[_nSamples];
        _Signs = new MeasureType[_nSamples];

        for (int i = 0; i < _nSamples; ++i){
            _Signal[i] = 0;
            _Signs[i] = 1;
        }
    }

    /**
     * _StartPoint is allocated here!!!
     * @param _nDims
     */
    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::setNDims(int _nDims){

        delete [] _StartPoint; _StartPoint = 0;

        Calculator::_nDims = _nDims;

        _StartPoint = new MeasureType[_nDims];

        for (int i = 0; i < _nDims; ++i){
            _StartPoint[i] = 0;
        }
    }

    template< typename MeasureType >
    void
    Calculator<MeasureType>
    ::disp(){
        int nSamples = this->getNSamples();
        std::cout << "\nYou called disp() on a Calculator object " << this
                  << " with nSamples: " << nSamples
                  << " nDims: " << _nDims;
        KWUtil::printArray(_InvTimes != 0, nSamples, _InvTimes,       (char*)"\nInvTimes:    ");
        KWUtil::printArray(_EchoTimes != 0, nSamples, _EchoTimes,     (char*)"\nEchoTimes:   ");
        KWUtil::printArray(_RepTimes != 0, nSamples, _RepTimes,       (char*)"\nRepTimes:    ");
        KWUtil::printArray(_RelAcqTimes != 0, nSamples, _RelAcqTimes, (char*)"\nRelAcqTimes: ");
        KWUtil::printArray(_SigMag != 0, nSamples, _SigMag,           (char*)"\nSigMag:      ");
        KWUtil::printArray(_SigPha != 0, nSamples, _SigPha,           (char*)"\nSigPha:      ");
        KWUtil::printArray(_Signal != 0, nSamples, _Signal,           (char*)"\nSignal:      ");
        KWUtil::printArray(_Signs != 0, nSamples, _Signs,             (char*)"\nSigns:       ");
        KWUtil::printArray(_StartPoint != 0, _nDims, _StartPoint,     (char*)"\nStart point: ");
        std::cout << std::endl;
        if(_Model) {
            std::cout << "This Calculator contains the following Model object: ";
            _Model->disp();
        }

        if(_Fitter) {
            std::cout << "This Calculator contains the following Fitter object: ";
            _Fitter->disp();
        }

        if(_SignCalculator) {
            std::cout << "This Calculator contains the following SignCalculator object: ";
            _SignCalculator->disp();
        };

        if(_StartPointCalculator) {
            std::cout << "This Calculator contains the following StartPointCalculator object: ";
            _StartPointCalculator->disp();
        };
    }

} //namespace Ox

#endif //Tomato_OXCALCULATORT1_H
