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
#include "OxModelT1AdapterVnlLeastSquares.h"
#include <vnl/algo/vnl_levenberg_marquardt.h>

namespace Ox {
    template<typename MeasureType>
    class FitterLevenbergMarquardtVnl : public Fitter<MeasureType> {

    public:
        typedef vnl_levenberg_marquardt VnlFitterType; // !!! this is the most important configuration part !!!
        typedef ModelT1AdapterVnlLeastSquares FunctionsAdaptedToVnlType;

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int performFitting(){

            configureMinimizer();

            vnl_vector<MeasureType> temp(this->getParameters(), this->_Model->getNDims());

            _VnlFitter->minimize(temp);

            if (temp.size() != 0) {
                temp.copy_out(this->getParameters());
            }
            if (this->getVerbose()) {
                std::cout << "Results: " << temp << " Cost: " << this->_Model->calcCostValue(this->getParameters()) << std::endl;
            }

            return 0; //EXIT_SUCCESS;
        };

        // getters
        FunctionsAdaptedToVnlType *getFunctionsAdaptedToVnl() const {
            return _FunctionsAdaptedToVnl;
        }

        /**
         * \brief show me your Fitter
         */
        virtual void disp(){
            std::cout << "\nYou called disp() on a FitterLevenbergMarquardtVnl object" << this << "\n";
            std::cout << "It has VnlFitter " << _VnlFitter << std::endl;
            std::cout << "It has FunctionsAdaptedToVnl " << _FunctionsAdaptedToVnl << std::endl;
            std::cout << "It's base class is as follows: ";

            Fitter<MeasureType>::disp();
        }

        /**
         * \brief constructor
         */
        FitterLevenbergMarquardtVnl() {
            // I cannot initialise _FunctionsAdaptedToVnl here, as I do not know nSamples yet
            _FunctionsAdaptedToVnl = 0; // nullptr
            _VnlFitter = 0; // nullptr
        };

        /**
         * copy constructor
         * @param old
         */
        FitterLevenbergMarquardtVnl(const FitterLevenbergMarquardtVnl &old) : Fitter<MeasureType>(old){
            // I cannot initialise _FunctionsAdaptedToVnl here, as I do not know nSamples and nDims yet
            _FunctionsAdaptedToVnl = 0; // nullptr
            _VnlFitter = 0; // nullptr
        }

        /**
         * cloning
         * @return
         */
        virtual Fitter<MeasureType> *newByCloning() { return new FitterLevenbergMarquardtVnl<MeasureType>(*this); }

        /**
         * \brief destructor
         */
        virtual ~FitterLevenbergMarquardtVnl() {
            delete _FunctionsAdaptedToVnl; _FunctionsAdaptedToVnl = 0;
            delete _VnlFitter; _VnlFitter = 0;
        };

    protected:

        virtual void configureMinimizer() {
            int nSamples = this->_Model->getNSamples();
            int nDims = this->_Model->getNDims();

            bool doReconfigure = false;

            if (!_VnlFitter) {
                doReconfigure = true;
            }

            if (_FunctionsAdaptedToVnl != 0) {
                if (nSamples != _FunctionsAdaptedToVnl->get_number_of_residuals()) {
                    doReconfigure = true;
                }
            }


            if (doReconfigure) {
                if (!this->_Model) {
                    throw std::runtime_error("Set the Model object");
                } else {

                    delete _FunctionsAdaptedToVnl; _FunctionsAdaptedToVnl = 0;

                    if (this->_UseGradient) {
                        _FunctionsAdaptedToVnl = new FunctionsAdaptedToVnlType(nDims, nSamples, vnl_least_squares_function::use_gradient);
                    } else {
                        _FunctionsAdaptedToVnl = new FunctionsAdaptedToVnlType(nDims, nSamples, vnl_least_squares_function::no_gradient);
                    }

                    _FunctionsAdaptedToVnl->setModel(this->_Model);

                    delete _VnlFitter; _VnlFitter = 0;
                    _VnlFitter = new VnlFitterType(*_FunctionsAdaptedToVnl);
                }
                _VnlFitter->set_x_tolerance(this->getXTolerance());
                _VnlFitter->set_f_tolerance(this->getFTolerance());
                //m_LocalFitter->set_g_tolerance(this->GetGTolerance());
                _VnlFitter->set_max_function_evals(this->getMaxFunctionEvals());
            }

        };

    private:

        VnlFitterType *_VnlFitter; // not meant to be see outside
        FunctionsAdaptedToVnlType *_FunctionsAdaptedToVnl; // not meant to be see outside

    };

} // namespace Ox

#endif //USE_VNL

#endif //Tomato_OXFITTERLevenbergMarquardtVNL_H
