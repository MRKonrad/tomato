/*!
 * \file OxFitterLevenbergMarquardtVnl.h
 * \author Konrad Werys
 * \date 2018/07/31
 */

#ifndef Tomato_OXFITTERLevenbergMarquardtVNL_H
#define Tomato_OXFITTERLevenbergMarquardtVNL_H

#include "CmakeConfigForTomato.h"
#ifdef USE_VNL

#include "OxFitter.h"

class vnl_levenberg_marquardt;

namespace Ox {

    class ModelT1AdapterVnlLeastSquares;

    template<typename MeasureType>
    class FitterLevenbergMarquardtVnl : public Fitter<MeasureType> {

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int performFitting();

        // getters
        ModelT1AdapterVnlLeastSquares *getModelAdaptedToVnl() const {
            return _FunctionsAdaptedToVnl;
        }

        /**
         * \brief show me your Fitter
         */
        virtual void disp();

        /**
         * \brief constructor
         */
        FitterLevenbergMarquardtVnl();

        /**
         * copy constructor
         * @param old
         */
        FitterLevenbergMarquardtVnl(const FitterLevenbergMarquardtVnl &old);

        /**
         * cloning
         * @return
         */
        virtual Fitter<MeasureType> *newByCloning() { return new FitterLevenbergMarquardtVnl<MeasureType>(*this); }

        /**
         * \brief destructor
         */
        virtual ~FitterLevenbergMarquardtVnl();

    protected:

        virtual void configureMinimizer();

    private:

        vnl_levenberg_marquardt *_VnlFitter; // not meant to be see outside
        ModelT1AdapterVnlLeastSquares *_FunctionsAdaptedToVnl; // not meant to be see outside

    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFitterLevenbergMarquardtVnl.hxx"
#endif // TOMATOLIB_COMPILED

#endif //USE_VNL

#endif //Tomato_OXFITTERLevenbergMarquardtVNL_H
