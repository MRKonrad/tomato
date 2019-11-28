/*!
 * \file OxCalculatorT2.h
 * \author Konrad Werys
 * \date 2019/11/05
 */

#ifndef Tomato_OXCALCULATORT2_H
#define Tomato_OXCALCULATORT2_H

#include "OxCalculator.h"
#include "tomatolib_export.h"

namespace Ox {

    /**
     * \class CalculatorT2
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class CalculatorT2 : public Calculator<MeasureType> {
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
         * /throw exception if _EchoTimes == 0
         * @return
         */
        const MeasureType * getEchoTimes() const;

        /**
         * constructor
         */
        CalculatorT2() : Calculator<MeasureType>(){}

        /**
         * destructor
         */
        virtual ~CalculatorT2(){}

        /**
         * cloning
         * @return
         */
        virtual Calculator<MeasureType> *newByCloning() { return new CalculatorT2<MeasureType>(*this); }


    };

} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxCalculatorT2.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXCALCULATORT2_H
