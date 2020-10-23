/*!
 * \file OxFitterLevenbergMarquardtLmfit.h
 * \author Konrad Werys
 * \date 2019/08/14
 */

#ifndef Tomato_OXFITTERLEVENBERGMARQUARDTLMFIT_HXX
#define Tomato_OXFITTERLEVENBERGMARQUARDTLMFIT_HXX

#include "CmakeConfigForTomato.h"
#ifdef USE_LMFIT

#include "OxFitter.h"
#include "OxModelT1AdapterLmfitLeastSquares.h"
#include "lmmin.h"

namespace Ox {

    template < typename TYPE >
    int
    FitterLevenbergMarquardtLmfit<TYPE>
    ::performFitting(){

        lm_control_struct control = lm_control_double;
        lm_status_struct status;

        // TODO: move to constructor to make it faster
        control.patience = this->getMaxFunctionEvals();
        control.ftol = this->getFTolerance();
        control.xtol = this->getXTolerance();
        //control.gtol = this->GetGTolerance();

        if (this->getVerbose()){
            control.verbosity = 1;
        }

        if (this->getTrace()){
            control.verbosity = 3;
        }

        lmmin(
                this->_Model->getNDims(),
                this->getParameters(),
                this->_Model->getNSamples(),
                NULL,
                (const void *)this->_Model,
                ModelT1AdapterLmfitLeastSquares<TYPE>::calcLSResiduals,
                &control,
                &status);

        return 0; //EXIT_SUCCESS;
    }

    template < typename TYPE >
    void
    FitterLevenbergMarquardtLmfit<TYPE>
    ::disp(){
        std::cout << "\nYou called disp() on a FitterLevenbergMarquardtLmfit object " << this << "\n";
        std::cout << "It's base class is as follows: ";

        Fitter<TYPE>::disp();
    }

} // namespace Ox

#endif //USE_LMFIT

#endif //Tomato_OXFITTERLEVENBERGMARQUARDTLMFIT_HXX
