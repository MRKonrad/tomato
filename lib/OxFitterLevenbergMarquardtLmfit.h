/*!
 * \file OxFitterLevenbergMarquardtLmfit.h
 * \author Konrad Werys
 * \date 2019/08/14
 */

#ifndef Tomato_OXFITTERLEVENBERGMARQUARDTLMFIT_H
#define Tomato_OXFITTERLEVENBERGMARQUARDTLMFIT_H

#include "CmakeConfigForTomato.h"
#ifdef USE_LMFIT

#include "OxFitter.h"

namespace Ox {
    template<typename MeasureType>
    class FitterLevenbergMarquardtLmfit : public Fitter<MeasureType> {

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int performFitting();

        /**
         * \brief show me your Fitter
         */
        virtual void disp();

        /**
         * \brief constructor
         */
        FitterLevenbergMarquardtLmfit() {};

        /**
         * cloning
         * @return
         */
        virtual Fitter<MeasureType> *newByCloning() { return new FitterLevenbergMarquardtLmfit<MeasureType>(*this); }

        /**
         * \brief destructor
         */
        virtual ~FitterLevenbergMarquardtLmfit() {};


    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFitterLevenbergMarquardtLmfit.hxx"
#endif // TOMATOLIB_COMPILED

#endif //USE_LMFIT

#endif //Tomato_OXFITTERLEVENBERGMARQUARDTLMFIT_H
