/*!
 * \file OxFitterAmoebaVnl.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef Tomato_OXFITTERAMOEBAVNL_H
#define Tomato_OXFITTERAMOEBAVNL_H

#include "CmakeConfigForTomato.h"
#ifdef USE_VNL

#include "OxFitter.h"

class vnl_amoeba;

namespace Ox {

    class ModelT1AdapterVnlCost;

    template<typename MeasureType>
    class FitterAmoebaVnl : public Fitter<MeasureType> {

    public:
        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int performFitting();

        // getters
        ModelT1AdapterVnlCost *getModelAdaptedToVnl() const;

        /**
         * \brief show me your Fitter
         */
        virtual void disp();

        /**
         * \brief constructor
         */
        FitterAmoebaVnl();

        /**
         * cloning
         * @return
         */
        virtual Fitter<MeasureType> *newByCloning() { return new FitterAmoebaVnl<MeasureType>(*this); }

        /**
         * \brief destructor
         */
        virtual ~FitterAmoebaVnl();

    protected:

        virtual void configureMinimizer();

    private:

        vnl_amoeba *_VnlFitter; // not meant to be see outside
        ModelT1AdapterVnlCost *_FunctionsAdaptedToVnl; // not meant to be see outside

    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFitterAmoebaVnl.hxx"
#endif // TOMATOLIB_COMPILED

#endif //USE_VNL

#endif //Tomato_OXFITTERAMOEBAVNL_H
