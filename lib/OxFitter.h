/*!
 * \file OxFitter.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef OXSHMOLLI2_OXFITTER_H
#define OXSHMOLLI2_OXFITTER_H

#include "OxFunctionsT1.h"

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

        virtual const FunctionsT1<MeasureType>* getFunctionsT1() const { return _FunctionsT1; }

        virtual const MeasureType getXTolerance() const { return _XTolerance; }
        virtual const MeasureType getFTolerance() const { return _FTolerance; }
        virtual const unsigned int getMaxFunctionEvals() const { return _MaxFunctionEvals; }
        virtual const unsigned int getThreadId() const { return _ThreadId; }
        virtual const bool getVerbose() const { return _Verbose; }
        virtual const bool getTrace() const { return _Trace; }

        virtual void setFunctionsT1(FunctionsT1<MeasureType>* _FunctionsT1) { Fitter::_FunctionsT1 = _FunctionsT1; }

        virtual void setXTolerance(const MeasureType _XTolerance) { Fitter::_XTolerance = _XTolerance; }
        virtual void setFTolerance(const MeasureType _FTolerance) { Fitter::_FTolerance = _FTolerance; }
        virtual void setMaxFunctionEvals(const unsigned int _MaxFunctionEvals) { Fitter::_MaxFunctionEvals = _MaxFunctionEvals; }
        virtual void setThreadId(const unsigned int _ThreadId) { Fitter::_ThreadId = _ThreadId; }
        virtual void setVerbose(const bool _Verbose) { Fitter::_Verbose = _Verbose; }
        virtual void setTrace(const bool _Trace) { Fitter::_Trace = _Trace; }

        /**
         * \brief show me your Fitter
         */
        virtual void disp(){
            std::cout << "\nYou called disp() on a Fitter object " << this << "\n";
            std::cout << "XTolerance:       " << getXTolerance() << std::endl;
            std::cout << "FTolerance:       " << getFTolerance() << std::endl;
            std::cout << "MaxFunctionEvals: " << getMaxFunctionEvals() << std::endl;
            std::cout << "ThreadId:         " << getThreadId() << std::endl;
            if(_FunctionsT1) {
                std::cout << "This Fitter contains the followind FunctionsT1 object: ";
                _FunctionsT1->disp();
            };
        }

        /**
         * \brief constructor
         */
        Fitter(){
            _FunctionsT1 = 0; //nullpointer

            _XTolerance = 1e-12;
            _FTolerance = 1e-12;
            _MaxFunctionEvals = 4000;
            _ThreadId = 0;
            _Verbose = false;
            _Trace = false;
        }

        /**
         * copy constructor
         * @param old
         */
        Fitter(const Fitter &old) {
            _FunctionsT1 = 0; //nullpointer

            _XTolerance = old._XTolerance;
            _FTolerance = old._FTolerance;
            _MaxFunctionEvals = old._MaxFunctionEvals;
            _ThreadId = old._ThreadId;
            _Verbose = old._Verbose;
            _Trace = old._Trace;
        }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~Fitter(){};

    protected:
        FunctionsT1<MeasureType>* _FunctionsT1;

        MeasureType _XTolerance;
        MeasureType _FTolerance;
        unsigned int _MaxFunctionEvals;
        unsigned int _ThreadId;
        bool _Verbose;
        bool _Trace;


    };
} //namespace Ox

#endif //OXSHMOLLI2_OXFITTER_H
