/*!
 * \file OxCalculatorT1WithSignCheck.h
 * \author Konrad Werys
 * \date 2018/08/01
 */

#ifndef Tomato_OXCALCULATORT1WithSignCheck_H
#define Tomato_OXCALCULATORT1WithSignCheck_H

#include "OxCalculator.h"
#include "tomatolib_export.h"

namespace Ox {

    /**
     * \class CalculatorT1WithSignCheck
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class CalculatorT1WithSignCheck : public Calculator<MeasureType> {
    public:

        /**
         * calling calculateWithSignCheck(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs)
         * @return success/failure
         */
        virtual int calculate();

        /**
         * do all the checks and  prepare to do the calculation, for example calc signal/signs and _TRRaverageHB
         * @return
         */
        virtual int prepareToCalculate();

        /**
         * The most important function of this class
         * It has all the input parameters so that I can call it from the shWithSignCheck class
         * @param nSamples
         * @param invTimes
         * @param signal
         * @param signs
         * @return results
         */
        virtual std::map <std::string, MeasureType> calculateWithSignCheck(int nSamples, const MeasureType* invTimes, MeasureType* signal, MeasureType* signs);

        /**
         * Calculate goodness of fit map
         * @param nSamples
         * @param invTimes
         * @param signal
         * @param parameters
         * @return
         */
        MeasureType calculateR2AbsFromModel(int nSamples, const MeasureType* invTimes, const MeasureType* signal, const MeasureType* parameters);

        /**
         * calculate covariance matrix needed for SD estimation
         * @param parameters
         * @param covarianceMatrix
         * @return
         */
        int calculateCovarianceMatrix(const MeasureType* parameters, MeasureType *covarianceMatrix);

        /**
         * calculate inverse covariance matrix needed for SD estimation
         * @param invTimes
         * @param residuals
         * @param parameters
         * @param invCovarianceMatrix
         * @return
         */
        int calculateInvCovarianceMatrix(const MeasureType* invTimes, const MeasureType* residuals, const MeasureType* parameters, MeasureType *invCovarianceMatrix);

        /**
         *
         * @return
         */
        bool getDoCalculateSDMap() const;


        /**
         * /throw exception if _InvTimes == 0
         * @return
         */
        const MeasureType * getInvTimes() const;

        /**
         *
         * @param _DoCalculateSDMap
         */
        void setDoCalculateSDMap(bool _DoCalculateSDMap);

        /**
         * constructor
         */
        CalculatorT1WithSignCheck() : Calculator<MeasureType>(){
            MaxTIForSignInvert = this->MAX_T1_TRESHOLD * 0.67;
            _DoCalculateSDMap = false;
        }

        /**
         * cloning
         * @return
         */
        virtual Calculator<MeasureType> *newByCloning() { return new CalculatorT1WithSignCheck<MeasureType>(*this); }

    protected:

        const static int MAX_T1_TRESHOLD = 4000;

        double MaxTIForSignInvert;
        bool _DoCalculateSDMap;

    };

} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxCalculatorT1WithSignCheck.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXCALCULATORT1WithSignCheck_H
