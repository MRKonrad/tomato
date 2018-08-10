/*!
 * \file OxCalculator.hxx
 * \author Konrad Werys
 * \date 2018/08/09
 */

#ifndef OXSHMOLLI2_OXCALCULATORT1_HXX
#define OXSHMOLLI2_OXCALCULATORT1_HXX

namespace Ox {

    /* ****************** */
    /* ***  GETTERS   *** */
    /* ****************** */

    template< typename MeasureType >
    FunctionsT1<MeasureType>*
    CalculatorT1<MeasureType>
    ::getFunctionsT1() const {
        if (!_FunctionsT1) {
            throw std::runtime_error("_FunctionsT1 equals 0. Set _FunctionsT1");
        };
        return _FunctionsT1;
    }

    template< typename MeasureType >
    Fitter<MeasureType> *
    CalculatorT1<MeasureType>
    ::getFitter() const {
        if (!_Fitter) {
            throw std::runtime_error("_Fitter equals 0. Set _Fitter");
        };
        return _Fitter;
    }

    template< typename MeasureType >
    StartPointCalculator<MeasureType> *
    CalculatorT1<MeasureType>
    ::getStartPointCalculator() const {
        if (!_StartPointCalculator) {
            throw std::runtime_error("_StartPointCalculator equals 0. Set _StartPointCalculator");
        };
        return _StartPointCalculator;
    }

    template< typename MeasureType >
    SignCalculator<MeasureType> *
    CalculatorT1<MeasureType>
    ::getSignCalculator() const {
        if (!_SignCalculator) {
            throw std::runtime_error("_SignCalculator equals 0. Set _SignCalculator");
        };
        return _SignCalculator;
    }

    template< typename MeasureType >
    const MeasureType *
    CalculatorT1<MeasureType>
    ::getInvTimes() const {
        if (!_InvTimes) {
            throw std::runtime_error("_InvTimes equals 0. Set _InvTimes");
        };
        return _InvTimes;
    }

//    template< typename MeasureType >
//    const MeasureType *
//    CalculatorT1<MeasureType>
//    ::getSatTimes() const {
//        return _SatTimes;
//    }

    template< typename MeasureType >
    const MeasureType *
    CalculatorT1<MeasureType>
    ::getRepTimes() const {
        return _RepTimes;
    }

    template< typename MeasureType >
    const MeasureType *
    CalculatorT1<MeasureType>
    ::getRelAcqTimes() const {
        return _RelAcqTimes;
    }

    template< typename MeasureType >
    const MeasureType *
    CalculatorT1<MeasureType>
    ::getSigMag() const {
        if (!_SigMag) {
            throw std::runtime_error("_SigMag equals 0. Set _SigMag" );
        };
        return _SigMag;
    }

    template< typename MeasureType >
    const MeasureType *
    CalculatorT1<MeasureType>
    ::getSigPha() const {
        return _SigPha;
    }

    template< typename MeasureType >
    MeasureType *
    CalculatorT1<MeasureType>
    ::getSignal() const {
        return _Signal;
    }

    template< typename MeasureType >
    MeasureType *
    CalculatorT1<MeasureType>
    ::getSigns() const {
        return _Signs;
    }

    template< typename MeasureType >
    MeasureType *
    CalculatorT1<MeasureType>
    ::getStartPoint()  {
        return _StartPoint;
    }

    template< typename MeasureType >
    const CalculatorT1Results<MeasureType>
    CalculatorT1<MeasureType>
    ::getResults() const {
        return _Results;
    }

    template< typename MeasureType >
    MeasureType
    CalculatorT1<MeasureType>
    ::getMeanCutOff() const {
        return _MeanCutOff;
    }

    template< typename MeasureType >
    int
    CalculatorT1<MeasureType>
    ::getNSamples() const {
        if (!_nSamples) {
            throw std::runtime_error("_nSamples equals 0. Set _nSamples");
        };
        return _nSamples;
    }

    template< typename MeasureType >
    int
    CalculatorT1<MeasureType>
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
    CalculatorT1<MeasureType>
    ::setFunctionsT1(FunctionsT1<MeasureType> *_FunctionsT1) {
        setNDims(_FunctionsT1->getNDims());
        CalculatorT1::_FunctionsT1 = _FunctionsT1;
    }

    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setFitter(Fitter<MeasureType> *_Fitter) {
        CalculatorT1::_Fitter = _Fitter;
    }

    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setSignCalculator(SignCalculator<MeasureType> *_SignCalculator) {
        CalculatorT1::_SignCalculator = _SignCalculator;
    }

    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setStartPointCalculator(StartPointCalculator<MeasureType> *_StartPointCalculator) {
        CalculatorT1::_StartPointCalculator = _StartPointCalculator;
    }

    // setters for the data
    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setInvTimes(const MeasureType *_InvTimes) {
        CalculatorT1::_InvTimes = _InvTimes;
    }

    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setSigMag(const MeasureType *_SigMag) {
        CalculatorT1::_SigMag = _SigMag;
    }

    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setSigPha(const MeasureType *_SigPha) {
        CalculatorT1::_SigPha = _SigPha;
    }

    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setMeanCutOff(MeasureType _MeanCutOff) {
        CalculatorT1::_MeanCutOff = _MeanCutOff;
    }

    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setNSamples(int _nSamples) {

        delete [] _Signal; _Signal = 0;
        delete [] _Signs; _Signs = 0;

        CalculatorT1::_nSamples = _nSamples;

        _Signal = new MeasureType[_nSamples];
        _Signs = new MeasureType[_nSamples];

        for (int i = 0; i < _nSamples; ++i){
            _Signal[i] = 0;
            _Signs[i] = 0;
        }
    }

    /**
     * _StartPoint is allocated here!!!
     * @param _nDims
     */
    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::setNDims(int _nDims){

        delete [] _StartPoint; _StartPoint = 0;

        CalculatorT1::_nDims = _nDims;

        _StartPoint = new MeasureType[_nDims];

        for (int i = 0; i < _nDims; ++i){
            _StartPoint[i] = 0;
        }
    }

    template< typename MeasureType >
    int
    CalculatorT1<MeasureType>
    ::prepareToCalculate(){

        // if fitter does not have to iterate, do not calculate
        if (this->getFitter()->getMaxFunctionEvals() == 0){
            return 1; // EXIT_FAILURE
        }

        // verify invTimes are sorted
        for (int i = 0; i < getNSamples()-1; i++){
            if (getInvTimes()[i] > getInvTimes()[i+1]){
                throw std::runtime_error("InvTimes have to be sorted!");
            }
        }

        // calculate sign
        getSignCalculator()->setNSamples(getNSamples());
        getSignCalculator()->setInvTimes(getInvTimes());
        getSignCalculator()->setSigMag(getSigMag());
        getSignCalculator()->setSigPha(getSigPha());
        getSignCalculator()->setSignal(_Signal);
        getSignCalculator()->setSigns(_Signs);

        getSignCalculator()->calculateSign();

        // calculate start point
        getStartPointCalculator()->setCalculatedStartPoint(_StartPoint);
        getStartPointCalculator()->calculateStartPoint();

        return 0; // EXIT_SUCCESS
    };


    template< typename MeasureType >
    void
    CalculatorT1<MeasureType>
    ::disp(){
        int nSamples = this->getNSamples();
        std::cout << "\nYou called disp() on a CalculatorT1 object " << this
                  << " with nSamples: " << nSamples
                  << " nDims: " << _nDims;
        KWUtil::printArray((bool)_InvTimes, nSamples, _InvTimes,       (char*)"\nInvTimes:    ");
        //KWUtil::printArray((bool)_SatTimes, nSamples, _SatTimes,       (char*)"\nSatTimes:    ");
        KWUtil::printArray((bool)_RepTimes, nSamples, _RepTimes,       (char*)"\nRepTimes:    ");
        KWUtil::printArray((bool)_RelAcqTimes, nSamples, _RelAcqTimes, (char*)"\nRelAcqTimes: ");
        KWUtil::printArray((bool)_SigMag, nSamples, _SigMag,           (char*)"\nSigMag:      ");
        KWUtil::printArray((bool)_SigPha, nSamples, _SigPha,           (char*)"\nSigPha:      ");
        KWUtil::printArray((bool)_Signal, nSamples, _Signal,           (char*)"\nSignal:      ");
        KWUtil::printArray((bool)_Signs, nSamples, _Signs,             (char*)"\nSigns:       ");
        KWUtil::printArray((bool)_nDims, getStartPoint(),              (char*)"\nStart point: ");
        std::cout << std::endl;
        if(_FunctionsT1) {
            std::cout << "This CalculatorT1 contains the following FunctionsT1 object: ";
            _FunctionsT1->disp();
        }

        if(_Fitter) {
            std::cout << "This CalculatorT1 contains the following Fitter object: ";
            _Fitter->disp();
        }

        if(_SignCalculator) {
            std::cout << "This CalculatorT1 contains the following SignCalculator object: ";
            _SignCalculator->disp();
        };

        if(_StartPointCalculator) {
            std::cout << "This CalculatorT1 contains the following StartPointCalculator object: ";
            _StartPointCalculator->disp();
        };

        _Results.disp();
    }


} //namespace Ox

#endif //OXSHMOLLI2_OXCALCULATORT1_H
