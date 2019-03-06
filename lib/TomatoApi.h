/*!
 * \file TomatoApi.h
 * \author Konrad Werys
 * \date 2019/01/15
 * This file is necessary to generate Tomato's dynamic library. Consider adding extern to the explicit instantiations
 * For more details refer to
 * http://mrkonrad.github.io/MRKonrad/CMakeDll
 * https://github.com/MRKonrad/dllPlayground
 * https://anteru.net/blog/2008/11/19/318/index.html
 */

#ifndef TOMATO_TOMATOAPI_H
#define TOMATO_TOMATOAPI_H

#include "CmakeConfigForTomato.h"
#include "tomatolib_export.h"

// calculators
#include "OxCalculatorT1.h"
#include "OxCalculatorT1Molli.h"
#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1Shmolli.h"
#include "OxCalculatorT1ShmolliOriginal.h"
#endif //USE_PRIVATE_NR2

// fitters
#include "OxFitter.h"
//#include "OxFitterAmoebaVnl.h"
//#include "OxFitterLevenbergMarquardtVnl.h"
#ifdef USE_PRIVATE_NR2
#include "OxFitterAmoebaPrivateNr2.h"
#endif //USE_PRIVATE_NR2

// functions
#include "OxFunctionsT1.h"
#include "OxFunctionsT1Basic.h"
#ifdef USE_PRIVATE_NR2
#include "OxFunctionsT1Shmolli.h"
#endif //USE_PRIVATE_NR2

// sign calculators
#include "OxSignCalculator.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#ifdef USE_PRIVATE_NR2
#include "OxSignCalculatorShmolli.h"
#endif //USE_PRIVATE_NR2

// start point calculators
#include "OxStartPointCalculatorDefault3Dims.h"
#ifdef USE_PRIVATE_NR2
#include "OxStartPointCalculatorShmolli.h"
#endif

namespace Ox {

    // calculators
    template class TOMATOLIB_EXPORT CalculatorT1<double>;
    template class TOMATOLIB_EXPORT CalculatorT1Molli<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT CalculatorT1Shmolli<double>;
    template class TOMATOLIB_EXPORT CalculatorT1ShmolliOriginal<double>;
#endif //USE_PRIVATE_NR2

    // fitters
    template class TOMATOLIB_EXPORT Fitter<double>;
    //template class TOMATOLIB_EXPORT FitterAmoebaVnl<double>;
    //template class TOMATOLIB_EXPORT FitterLevenbergMarquardtVnl<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT FitterAmoebaPrivateNr2<double>;
#endif //USE_PRIVATE_NR2

    // functions
    template class TOMATOLIB_EXPORT FunctionsT1<double>;
    template class TOMATOLIB_EXPORT FunctionsT1Basic<double>;
    #ifdef USE_PRIVATE_NR2
        template class TOMATOLIB_EXPORT FunctionsT1Shmolli<double>;
    #endif //USE_PRIVATE_NR2

    // sign calculators
    template class TOMATOLIB_EXPORT SignCalculator<double>;
    template class TOMATOLIB_EXPORT SignCalculatorNoSign<double>;
    template class TOMATOLIB_EXPORT SignCalculatorRealImag<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT SignCalculatorShmolli<double>;
#endif //USE_PRIVATE_NR2

    // start point calculators
    template class TOMATOLIB_EXPORT StartPointCalculator<double>;
    template class TOMATOLIB_EXPORT StartPointCalculatorDefault3Dims<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT StartPointCalculatorShmolli<double>;
#endif

} //namespace Ox

#endif //TOMATO_TOMATOAPI_H
