/*!
 * \file OxTestData.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef OXShmolli2_OXTESTDATA_H
#define OXShmolli2_OXTESTDATA_H

#include "KWUtil.h"

#include "yaml.h"


namespace Ox {

    template< typename MeasureType >
    class TestData {

    public:

        TestData(char* filePath);

        virtual std::vector<MeasureType> getSignalMag() const { return _signalMag; }
        virtual std::vector<MeasureType> getSignalPha() const { return _signalPha; }
        virtual std::vector<MeasureType> getSigns()     const { return _signs; }
        virtual std::vector<MeasureType> getSignal()    const { return _signal; }
        virtual std::vector<MeasureType> getInvTimes()  const { return _invTimes; }
        virtual std::vector<MeasureType> getResultsMolli()    const { return _resultsMolli; }
        virtual std::vector<MeasureType> getResultsShmolli()  const { return _resultsShmolli; }


        virtual const MeasureType* getSignalMagPtr() const { return &_signalMag.at(0); }
        virtual const MeasureType* getSignalPhaPtr() const { return &_signalPha.at(0); }
        virtual const MeasureType* getSignsPtr()     const { return &_signs.at(0); }
        virtual const MeasureType* getSignalPtr()    const { return &_signal.at(0); }
        virtual const MeasureType* getInvTimesPtr()  const { return &_invTimes.at(0); }
        virtual const MeasureType* getResultsMolliPtr()    const { return &_resultsMolli.at(0); }
        virtual const MeasureType* getResultsShmolliPtr()  const { return &_resultsShmolli.at(0); }

        virtual int getNSamples()  const { return _nSamples; }

        void copyStrVectorToMemberVector(std::vector<std::string> strVector, std::vector<MeasureType> &memberVector);

        void disp();

        template< typename TYPE >
        void printVector(std::vector<TYPE> myVector, std::string myVectorName);

    protected:
        int _nSamples;
        std::vector<MeasureType> _signalMag;
        std::vector<MeasureType> _signalPha;
        std::vector<MeasureType> _signal;
        std::vector<MeasureType> _signs;
        std::vector<MeasureType> _invTimes;

        std::vector<MeasureType> _resultsMolli;
        std::vector<MeasureType> _resultsShmolli;

        void calcSignal();
    };
} // namespace Ox

#include "OxTestData.hxx"

#endif //OXShmolli2_OXTESTDATA_H
