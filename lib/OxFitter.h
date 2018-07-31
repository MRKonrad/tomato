/*!
 * \file OxFitter.h
 * \author Konrad Werys
 * \date 2018/08/29
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
         * the function that is doing all the heavy lifting
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

        ~Fitter(){};

    protected:
        FunctionsT1<MeasureType>* _FunctionsT1;

        MeasureType _XTolerance;
        MeasureType _FTolerance;
        unsigned int _MaxFunctionEvals;
        unsigned int _ThreadId;
        bool _Verbose;
        bool _Trace;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */


    };
} //namespace Ox

#endif //OXSHMOLLI2_OXFITTER_H
