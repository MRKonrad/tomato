/*!
 * \file OxModel.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OxModel_HXX
#define Tomato_OxModel_HXX

#include "KWUtil.h"
#include <iostream>

namespace Ox {

    template< typename MeasureType >
    void
    Model<MeasureType>
    ::setNSamples(int _nSamples) {
        Model::_nSamples = _nSamples;
        _Residuals = new MeasureType[_nSamples];
    }

    template< typename MeasureType >
    void
    Model<MeasureType>
    ::disp(){
        int nSamples = this->getNSamples();
        std::cout << "\nYou called disp() on a Model object " << this << " with nSamples: " << getNSamples();
        KWUtil::printArray(_InvTimes   != 0, nSamples, _InvTimes,    (char*)"\nInvTimes:    ");
        KWUtil::printArray(_EchoTimes  != 0, nSamples, _EchoTimes,   (char*)"\nEchoTimes:   ");
        KWUtil::printArray(_RepTimes   != 0, nSamples, _RepTimes,    (char*)"\nRepTimes:    ");
        KWUtil::printArray(_RelAcqTimes!= 0, nSamples, _RelAcqTimes, (char*)"\nRelAcqTimes: ");
        KWUtil::printArray(_Signal     != 0, nSamples, _Signal,      (char*)"\nSignal:      ");
        std::cout << std::endl;
    }

    template< typename MeasureType >
    void
    Model<MeasureType>
    ::setAllPointersToNull(){
        _InvTimes = 0;
        _EchoTimes = 0;
        _RepTimes = 0;
        _RelAcqTimes = 0;
        _Signal = 0;
    }

    template< typename MeasureType >
    Model<MeasureType>
    ::Model(){

        _nSamples = 0;
        _nDims = 0;

        _Residuals = 0; // pointer
        setAllPointersToNull();

    };

    template< typename MeasureType >
    Model<MeasureType>
    ::Model(const Model &old) {

        _nSamples = old._nSamples;
        _nDims = old._nDims;

        _Residuals = new MeasureType[_nSamples];
        setAllPointersToNull();

    }

    template< typename MeasureType >
    Model<MeasureType>
    ::~Model(){
        delete [] _Residuals;
    };

} //namespace Ox

#endif //Tomato_OxModel_HXX
