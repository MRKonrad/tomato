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
#include "OxModelT1AdapterLmfitLeastSquares.h"
#include "lmmin.h"

namespace Ox {
    template<typename MeasureType>
    class FitterLevenbergMarquardtLmfit : public Fitter<MeasureType> {

    public:

        /**
         * the most important function of this class
         * TODO: this solution with passing void* and using reinterpret_cast is somewhat ugly. Any ideas how to make it nicer?
         * @return success/failure
         */
        virtual int performFitting(){

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
                    ModelT1AdapterLmfitLeastSquares<MeasureType>::calcLSResiduals,
                    &control,
                    &status);

            return 0; //EXIT_SUCCESS;
        };

        /**
         * \brief show me your Fitter
         */
        virtual void disp(){
            std::cout << "\nYou called disp() on a FitterLevenbergMarquardtLmfit object " << this << "\n";
            std::cout << "It's base class is as follows: ";

            Fitter<MeasureType>::disp();
        }

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

#endif //USE_LMFIT

#endif //Tomato_OXFITTERLEVENBERGMARQUARDTLMFIT_H
