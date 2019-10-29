/*!
 * \file OxTestData.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef Tomato_OXTESTDATA_H
#define Tomato_OXTESTDATA_H

#include "TomatoParser.h"

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
        virtual std::vector<MeasureType> getResultsTwoParam()  const { return _resultsTwoParam; }
        virtual std::vector<MeasureType> getResultsThreeParam()  const { return _resultsThreeParam; }


        virtual const MeasureType* getSignalMagPtr() const {
            if (_signalMag.size() == 0) {
                throw std::runtime_error("Empty signalMag");
            }
            return &_signalMag.at(0);
        }
        virtual const MeasureType* getSignalPhaPtr() const {
            if (_signalPha.size() == 0) {
                throw std::runtime_error("Empty signalPha");
            }
            return &_signalPha.at(0);
        }
        virtual const MeasureType* getSignsPtr()     const {
            if (_signs.size() == 0) {
                throw std::runtime_error("Empty signs");
            }
            return &_signs.at(0);
        }
        virtual const MeasureType* getSignalPtr()    const {
            if (_signal.size() == 0) {
                throw std::runtime_error("Empty signal");
            }
            return &_signal.at(0);
        }
        virtual const MeasureType* getInvTimesPtr()  const {
            if (_invTimes.size() == 0) {
                throw std::runtime_error("Empty invTimes");
            }
            return &_invTimes.at(0);
        }
        virtual const MeasureType* getResultsMolliPtr()    const {
            if (_resultsMolli.size() == 0) {
                throw std::runtime_error("Empty resultsMolli");
            }
            return &_resultsMolli.at(0);
        }
        virtual const MeasureType* getResultsShmolliPtr()  const {
            if (_resultsShmolli.size() == 0) {
                throw std::runtime_error("Empty resultsShmolli");
            }
            return &_resultsShmolli.at(0);
        }
        virtual const MeasureType* getResultsTwoParamPtr()  const {
            if (_resultsTwoParam.size() == 0) {
                throw std::runtime_error("Empty resultsTwoParam");
            }
            return &_resultsTwoParam.at(0);
        }
        virtual const MeasureType* getResultsThreeParamPtr()  const {
            if (_resultsThreeParam.size() == 0) {
                throw std::runtime_error("Empty resultsThreeParam");
            }
            return &_resultsThreeParam.at(0);
        }

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
        std::vector<MeasureType> _resultsTwoParam;
        std::vector<MeasureType> _resultsThreeParam;

        void calcSignal();
    };
} // namespace Ox

#include "OxTestData.hxx"

#endif //Tomato_OXTESTDATA_H
