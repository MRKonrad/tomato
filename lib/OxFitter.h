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
        virtual int minimize() = 0;

    protected:
        FunctionsT1<MeasureType>* _Functions;

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
        ~FunctionsT1(){};

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXFITTER_H
