/*!
 * \file OxFitter.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXFITTER_H
#define Tomato_OXFITTER_H

#include "OxModel.h"
#include "KWUtil.h"

namespace Ox {

    /**
     * \class Fitter
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class Fitter{

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int performFitting() = 0;

        virtual const Model<MeasureType>* getModel() const { return _ModelT1; }
        virtual MeasureType *getParameters() { return _Parameters; }
        virtual const MeasureType getXTolerance() const { return _XTolerance; }
        virtual const MeasureType getFTolerance() const { return _FTolerance; }
        virtual const bool getUseGradient() const { return _UseGradient; }
        virtual const unsigned int getMaxFunctionEvals() const { return _MaxFunctionEvals; }
        virtual const unsigned int getThreadId() const { return _ThreadId; }
        virtual const bool getVerbose() const { return _Verbose; }
        virtual const bool getTrace() const { return _Trace; }

        virtual void setModel(Model<MeasureType>* _ModelT1) { Fitter::_ModelT1 = _ModelT1; }
        virtual void setParameters( MeasureType *_Parameters) { Fitter::_Parameters = _Parameters; }
        virtual void setXTolerance(const MeasureType _XTolerance) { Fitter::_XTolerance = _XTolerance; }
        virtual void setUseGradient(const bool _UseGradient) { Fitter::_UseGradient = _UseGradient; }
        virtual void setFTolerance(const MeasureType _FTolerance) { Fitter::_FTolerance = _FTolerance; }
        virtual void setMaxFunctionEvals(const unsigned int _MaxFunctionEvals) { Fitter::_MaxFunctionEvals = _MaxFunctionEvals; }
        virtual void setThreadId(const unsigned int _ThreadId) { Fitter::_ThreadId = _ThreadId; }
        virtual void setVerbose(const bool _Verbose) { Fitter::_Verbose = _Verbose; }
        virtual void setTrace(const bool _Trace) { Fitter::_Trace = _Trace; }

        /**
         * copy from ptrFrom to the parameters. Parameters have to be allocated first
         * @param ptrFrom
         */
        virtual void copyToParameters(const MeasureType *ptrFrom){
            if (!Fitter::_ModelT1) {
                throw std::runtime_error("_Model equals 0. Set _Model");
            }

            if (!_Parameters){
                throw std::runtime_error("_Parameters equals 0. Set _Parameters");
            }

            for (int i = 0; i < Fitter::_ModelT1->getNDims(); ++i) {
                _Parameters[i] = ptrFrom[i];
            }
        }

        /**
         * \brief show me your Fitter
         */
        virtual void disp(){
            std::cout << "\nYou called disp() on a Fitter object " << this << "\n";
            std::cout << "XTolerance:       " << getXTolerance() << std::endl;
            std::cout << "FTolerance:       " << getFTolerance() << std::endl;
            std::string temp = getUseGradient() ? "true" : "false";
            std::cout << "UseGradient       " << temp << std::endl;
            std::cout << "MaxFunctionEvals: " << getMaxFunctionEvals() << std::endl;
            std::cout << "ThreadId:         " << getThreadId() << std::endl;
            if(_ModelT1) {
                KWUtil::printArray(_Parameters, _ModelT1->getNDims(), _Parameters,     (char*)"\nParameters:  ");
                std::cout << "This Fitter contains the following Model object: ";
                _ModelT1->disp();
            };
        }

        /**
         * \brief constructor
         */
        Fitter(){
            _ModelT1 = 0; //nullpointer
            _Parameters = 0; //nullpointer

            _XTolerance = 1e-12;
            _FTolerance = 1e-12;
            _MaxFunctionEvals = 4000;
            _UseGradient = false;
            _ThreadId = 0;
            _Verbose = false;
            _Trace = false;
        }

        /**
         * copy constructor
         * @param old
         */
        Fitter(const Fitter &old) {
            _ModelT1 = 0; //nullpointer

            _XTolerance = old._XTolerance;
            _FTolerance = old._FTolerance;
            _MaxFunctionEvals = old._MaxFunctionEvals;
            _UseGradient = old._UseGradient;
            _ThreadId = old._ThreadId;
            _Verbose = old._Verbose;
            _Trace = old._Trace;
        }

        /**
         * cloning
         * @return
         */
        virtual Fitter<MeasureType> *newByCloning() = 0;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~Fitter(){};

    protected:
        Model<MeasureType>* _ModelT1;
        MeasureType* _Parameters;

        MeasureType _XTolerance;
        MeasureType _FTolerance;
        unsigned int _MaxFunctionEvals;
        bool _UseGradient;
        unsigned int _ThreadId;
        bool _Verbose;
        bool _Trace;

    };
} //namespace Ox

#endif //Tomato_OXFITTER_H
