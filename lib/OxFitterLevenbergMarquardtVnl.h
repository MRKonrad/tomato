/*!
 * \file OxFitterLevenbergMarquardtVnl.h
 * \author Konrad Werys
 * \date 2018/08/31
 */

#ifndef OXSHMOLLI2_OXFITTERLevenbergMarquardtVNL_H
#define OXSHMOLLI2_OXFITTERLevenbergMarquardtVNL_H

#include "OxFitter.h"
#include "OxFunctionsT1AdapterVnlLeastSquares.h"
#include <vnl/algo/vnl_levenberg_marquardt.h>

// TODO: look at the gradient. Interface it.

namespace Ox {
    template<typename MeasureType>
    class FitterLevenbergMarquardtVnl : public Fitter<MeasureType> {

    public:
        typedef vnl_levenberg_marquardt VnlFitterType; // !!! this is the most important configuration part !!!
        typedef FunctionsT1AdapterVnlLeastSquares FunctionsAdaptedToVnlType;

        FitterLevenbergMarquardtVnl() {
            _VnlFitter = 0; // nullpointer
            _FunctionsAdaptedToVnl = 0; // nullpointer
        };

        virtual ~FitterLevenbergMarquardtVnl() {
            delete _FunctionsAdaptedToVnl;
            delete _VnlFitter;
        };

        virtual int performFitting(){
            configureMinimizer();

            vnl_vector<MeasureType> temp(this->_FunctionsT1->getParameters(), 3);

            _VnlFitter->minimize(temp);

            if (temp.size() != 0) {
                this->_FunctionsT1->setParameters(temp.data_block());
            }
            if (this->getVerbose()) {
                std::cout << "Results: " << temp << " Cost: " << this->_FunctionsT1->calcCostValue() << std::endl;
            }

            return EXIT_SUCCESS;
        };

    protected:

        virtual void configureMinimizer() {
            if (!_VnlFitter) {
                if (!this->_FunctionsT1) {
                    std::cerr << "Set the OxFunctions object" << std::endl;
                    throw std::exception();
                } else {
                    delete _FunctionsAdaptedToVnl;
                    delete _VnlFitter;
                    int nSamples = this->_FunctionsT1->getNSamples();
                    _FunctionsAdaptedToVnl = new FunctionsAdaptedToVnlType(nSamples, vnl_least_squares_function::use_gradient);
                    _FunctionsAdaptedToVnl->setFunctionsT1(this->_FunctionsT1);
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

#endif //OXSHMOLLI2_OXFITTERLevenbergMarquardtVNL_H
