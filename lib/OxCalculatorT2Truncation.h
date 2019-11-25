/*!
 * \file OxCalculatorT2Truncation.h
 * \author Konrad Werys
 * \date 2019/11/25
 */

#ifndef Tomato_OXCalculatorT2Truncation_H
#define Tomato_OXCalculatorT2Truncation_H

#include "OxCalculator.h"
#include "tomatolib_export.h"

namespace Ox {

    /**
     * \class CalculatorT2Truncation
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class CalculatorT2Truncation : public Calculator<MeasureType> {
    public:

        /**
         * do all the checks and  prepare to do the calculation, for example calc signal/signs and _TRRaverageHB
         * @return success/failure
         */
        virtual int prepareToCalculate();

        /**
         * calling calculateMolli(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs)
         * @return success/failure
         */
        virtual int calculate();

        /**
         * Calculate goodness of fit map
         * @param nSamples
         * @param invTimes
         * @param signal
         * @param parameters
         * @return
         */
        MeasureType calculateR2AbsFromModel(int nSamples, const MeasureType* time, const MeasureType* signal, const MeasureType* parameters);


        /**
         * constructor
         */
        CalculatorT2Truncation() : Calculator<MeasureType>(){
            _SignalAboveSnrLimit = 0;
            _EchoTimesOfSignalAboveSnrLimit = 0;
            _NSamplesAboveSnrLimit = 0;
            _NoiseLimit = 2.;
        }

        /**
         * destructor
         */
        virtual ~CalculatorT2Truncation(){
            delete [] _SignalAboveSnrLimit;
            delete [] _EchoTimesOfSignalAboveSnrLimit;
        }

        /**
         * cloning
         * @return
         */
        virtual Calculator<MeasureType> *newByCloning() { return new CalculatorT2Truncation<MeasureType>(*this); }

    private:
        MeasureType _NoiseLimit;
        MeasureType *_SignalAboveSnrLimit; // TODO: Static allocation
        MeasureType *_EchoTimesOfSignalAboveSnrLimit; // TODO: Static allocation
        size_t _NSamplesAboveSnrLimit;

    };

} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxCalculatorT2Truncation.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXCalculatorT2Truncation_H
