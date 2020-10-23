/*!
 * \file OxFitterAmoebaVnl.h
 * \author Konrad Werys
 * \date 2020/10/23
 */

#ifndef Tomato_OXFITTERAMOEBAVNL_HXX
#define Tomato_OXFITTERAMOEBAVNL_HXX

#include "CmakeConfigForTomato.h"
#ifdef USE_VNL

#include "OxModelT1AdapterVnlCost.h"
#include <vnl/algo/vnl_amoeba.h>

namespace Ox {

    template<typename MeasureType>
    int
    FitterAmoebaVnl<MeasureType>
    ::performFitting(){

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
    }

    template<typename MeasureType>
    ModelT1AdapterVnlCost*
    FitterAmoebaVnl<MeasureType>
    ::getModelAdaptedToVnl() const {
        return _FunctionsAdaptedToVnl;
    }

    template<typename MeasureType>
    void
    FitterAmoebaVnl<MeasureType>
    ::disp(){
        std::cout << "\nYou called disp() on a FitterAmoebaVnl object " << this << "\n";
        std::cout << "It has VnlFitter " << _VnlFitter << std::endl;
        std::cout << "It has FunctionsAdaptedToVnl " << _FunctionsAdaptedToVnl << std::endl;
        std::cout << "It's base class is as follows: ";

        Fitter<MeasureType>::disp();
    }

    template<typename MeasureType>
    FitterAmoebaVnl<MeasureType>
    ::FitterAmoebaVnl() {
        // I cannot initialise _FunctionsAdaptedToVnl here, as I do not know nDims yet
        _FunctionsAdaptedToVnl = 0;
        _VnlFitter = 0;
    }

    template<typename MeasureType>
    FitterAmoebaVnl<MeasureType>
    ::~FitterAmoebaVnl() {
        delete _FunctionsAdaptedToVnl; _FunctionsAdaptedToVnl = 0;
        delete _VnlFitter; _VnlFitter = 0;
    }

    template<typename MeasureType>
    void
    FitterAmoebaVnl<MeasureType>
    ::configureMinimizer() {
        if (!_VnlFitter) {
            if (!this->_Model) {
                throw std::runtime_error("Set the Model object");
            } else {
                delete _FunctionsAdaptedToVnl; _FunctionsAdaptedToVnl = 0;
                delete _VnlFitter; _VnlFitter = 0;
                int nDims = this->_Model->getNDims();
                _FunctionsAdaptedToVnl = new ModelT1AdapterVnlCost(nDims);
                _FunctionsAdaptedToVnl->setModel(this->_Model);
                _VnlFitter = new vnl_amoeba(*_FunctionsAdaptedToVnl);
            }
            _FunctionsAdaptedToVnl->setModel(this->_Model);
            _VnlFitter->set_x_tolerance(this->getXTolerance());
            _VnlFitter->set_f_tolerance(this->getFTolerance());
            //m_LocalFitter->set_g_tolerance(this->GetGTolerance());
            _VnlFitter->set_max_iterations(this->getMaxFunctionEvals());
        }
    }

} // namespace Ox

#endif //USE_VNL

#endif //Tomato_OXFITTERAMOEBAVNL_H
