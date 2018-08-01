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

        /**
         * do all the checks and  prepare to do the calculation, for example calc signal/signs and _TRRaverageHB
         * @return success/failure
         */
        virtual int prepareToCalculate() = 0;

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculate() = 0;

        /**
         * \brief default constructor
         */
        CalculatorT1(){
            _Fitter = 0;
            _Functions = 0;
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
            _TRRaverageHB = 0;
            _MeanCutOff = 0;
        };

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~CalculatorT1(){};

    protected:
        Fitter<MeasureType>* _Fitter;
        FunctionsT1<MeasureType>* _Functions;
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

        MeasureType _TRRaverageHB;
        MeasureType _MeanCutOff;

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXCALCULATORT1_H
