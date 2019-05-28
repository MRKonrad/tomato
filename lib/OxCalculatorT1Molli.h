/*!
 * \file OxCalculatorT1Molli.h
 * \author Konrad Werys
 * \date 2018/08/01
 */

#ifndef Tomato_OXCALCULATORT1MOLLI_H
#define Tomato_OXCALCULATORT1MOLLI_H

#include "OxCalculatorT1.h"
#include "tomatolib_export.h"

namespace Ox {

    /**
     * \class CalculatorT1Molli
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class CalculatorT1Molli : public CalculatorT1<MeasureType> {
    public:

        /**
         * calling calculateMolli(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs)
         * @return success/failure
         */
        virtual int calculate();

        /**
         * The most important function of this class
         * It has all the input parametrs so that I can call it from the shmolli class
         * @param nSamples
         * @param invTimes
         * @param signal
         * @param signs
         * @return success/failure
         */
        virtual CalculatorT1Results<MeasureType> calculateMolli(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs);

        MeasureType calculateR2AbsFromModel(int nSamples, const MeasureType* invTimes, const MeasureType* signal, const MeasureType* parameters);

        int calculateCovarianceMatrix(const MeasureType* parameters, MeasureType *covarianceMatrix);

        int calculateInvCovarianceMatrix(const MeasureType* invTimes, const MeasureType* residuals, const MeasureType* parameters, MeasureType *invCovarianceMatrix);

        /**
         *
         * @return
         */
        bool getDoCalculateSDMap() const;

        /**
         *
         * @param _DoCalculateSDMap
         */
        void setDoCalculateSDMap(bool _DoCalculateSDMap);

        /**
         * constructor
         */
        CalculatorT1Molli() : CalculatorT1<MeasureType>(){
            MaxTIForSignInvert = this->MAX_T1_TRESHOLD * 0.67;
            _DoCalculateSDMap = false;
        }

        /**
         * cloning
         * @return
         */
        virtual CalculatorT1<MeasureType> *newByCloning() { return new CalculatorT1Molli<MeasureType>(*this); }



    protected:
        double MaxTIForSignInvert;
        bool _DoCalculateSDMap;
    };

} //namespace Ox

#ifdef TomatoLib_EXPORTS
#include "OxCalculatorT1Molli.hxx"
#endif //TomatoLib_EXPORTS

#endif //Tomato_OXCALCULATORT1MOLLI_H
