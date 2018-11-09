/*!
 * \file OxFitterAmoebaVnl.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef Tomato_OXFITTERAMOEBAVNL_H
#define Tomato_OXFITTERAMOEBAVNL_H

#include "OxFitter.h"
#include "OxFunctionsT1AdapterVnlCost.h"
#include <vnl/algo/vnl_amoeba.h>

namespace Ox {
    template<typename MeasureType>
    class FitterAmoebaVnl : public Fitter<MeasureType> {

    public:

        typedef vnl_amoeba VnlFitterType; // !!! this is the most important configuration part !!!
        typedef FunctionsT1AdapterVnlCost FunctionsAdaptedToVnlType;

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int performFitting(){

            configureMinimizer();

            vnl_vector<MeasureType> temp(this->getParameters(), this->_FunctionsT1->getNDims());

            _VnlFitter->minimize(temp);

            if (temp.size() != 0) {
                temp.copy_out(this->getParameters());
            }
            if (this->getVerbose()) {
                std::cout << "Results: " << temp << " Cost: " << this->_FunctionsT1->calcCostValue(this->getParameters()) << std::endl;
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
            std::cout << "\nYou called disp() on a FitterAmoebaVnl object " << this << "\n";
            std::cout << "It has VnlFitter " << _VnlFitter << std::endl;
            std::cout << "It has FunctionsAdaptedToVnl " << _FunctionsAdaptedToVnl << std::endl;
            std::cout << "It's base class is as follows: ";

            Fitter<MeasureType>::disp();
        }

        /**
         * \brief constructor
         */
        FitterAmoebaVnl() {
            // I cannot initialise _FunctionsAdaptedToVnl here, as I do not know nDims yet
            _FunctionsAdaptedToVnl = 0;
            _VnlFitter = 0;
        };

        /**
         * cloning
         * @return
         */
        virtual Fitter<MeasureType> *newByCloning() { return new FitterAmoebaVnl<MeasureType>(*this); }

        /**
         * \brief destructor
         */
        virtual ~FitterAmoebaVnl() {
            delete _FunctionsAdaptedToVnl; _FunctionsAdaptedToVnl = 0;
            delete _VnlFitter; _VnlFitter = 0;
        };

    protected:

        virtual void configureMinimizer() {
            if (!_VnlFitter) {
                if (!this->_FunctionsT1) {
                    throw std::runtime_error("Set the FunctionsT1 object");
                } else {
                    delete _FunctionsAdaptedToVnl; _FunctionsAdaptedToVnl = 0;
                    delete _VnlFitter; _VnlFitter = 0;
                    int nDims = this->_FunctionsT1->getNDims();
                    _FunctionsAdaptedToVnl = new FunctionsAdaptedToVnlType(nDims);
                    _FunctionsAdaptedToVnl->setFunctionsT1(this->_FunctionsT1);
                    _VnlFitter = new VnlFitterType(*_FunctionsAdaptedToVnl);
                }
                _FunctionsAdaptedToVnl->setFunctionsT1(this->_FunctionsT1);
                _VnlFitter->set_x_tolerance(this->getXTolerance());
                _VnlFitter->set_f_tolerance(this->getFTolerance());
                //m_LocalFitter->set_g_tolerance(this->GetGTolerance());
                _VnlFitter->set_max_iterations(this->getMaxFunctionEvals());
            }
        };

    private:

        VnlFitterType *_VnlFitter; // not meant to be see outside
        FunctionsAdaptedToVnlType *_FunctionsAdaptedToVnl; // not meant to be see outside

    };

} // namespace Ox

#endif //Tomato_OXFITTERAMOEBAVNL_H
