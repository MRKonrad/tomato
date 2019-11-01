/*!
 * \file OxCalculatorT1Shmolli.h
 * \author Konrad Werys
 * \date 2018/08/02
 */

#ifndef Tomato_OXCalculatorT1Shmolli_H
#define Tomato_OXCalculatorT1Shmolli_H

#include "tomatolib_export.h"
#include "OxCalculatorT1Molli.h"

namespace Ox {

    /**
     * \class OxCalculatorT1Shmolli
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class CalculatorT1Shmolli : public CalculatorT1Molli<MeasureType> {
    public:

        /**
         * constructor
         */
        CalculatorT1Shmolli() :  CalculatorT1Molli<MeasureType>(){
            _TRRaverageHB = 0;
        }

        /**
         * cloning
         * @return
         */
        virtual CalculatorT1<MeasureType> *newByCloning() { return new CalculatorT1Shmolli<MeasureType>(*this); }

        /**
         * prepareToCalculate
         * @return
         */
        virtual int prepareToCalculate();

        int getStartPointSKPShmolli(const MeasureType * invTimes5, const MeasureType * invTimes6, const MeasureType * invTimes7,
                                    const MeasureType * sigMag5,   const MeasureType * sigMag6,   const MeasureType * sigMag7,
                                    const MeasureType * signs5,    const MeasureType * signs6,    const MeasureType * signs7);

        /**
         * Average heart rate (distance between dwo R waves) TRR is used in ShMOLLI conditions
         * @return
         */
        int calculateTRRaverageHB();

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculate();

        int getShmolliSamples(const MeasureType* inArray, MeasureType* outArray5, MeasureType* outArray6, MeasureType* outArray7);

    private:
        MeasureType _TRRaverageHB;
    };

} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxCalculatorT1Shmolli.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXCalculatorT1Shmolli_H
