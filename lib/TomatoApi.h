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

/* ************* */
/*  calculators  */
/* ************* */
#include "OxCalculator.h"
#include "OxCalculatorT1Molli.h"
#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1Shmolli.h"
#include "OxCalculatorT1ShmolliOriginal.h"
#endif //USE_PRIVATE_NR2

/* ************* */
/*    fitters    */
/* ************* */
#include "OxFitter.h"
#ifdef USE_ITK
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#endif // USE_ITK
#ifdef USE_PRIVATE_NR2
#include "OxFitterAmoebaPrivateNr2.h"
#endif //USE_PRIVATE_NR2
#ifdef USE_LMFIT
#include "OxFitterLevenbergMarquardtLmfit.h"
#endif // USE_LMFIT

/* ************* */
/*   functions   */
/* ************* */
#include "OxModel.h"
#include "OxModelT1ThreeParam.h"
#ifdef USE_PRIVATE_NR2
#include "OxModelT1Shmolli.h"
#endif //USE_PRIVATE_NR2
#ifdef USE_LMFIT
#include "OxModelT1AdapterLmfitLeastSquares.h"
#endif // USE_LMFIT

/* **************** */
/* sign calculators */
/* **************** */
#include "OxSignCalculator.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#ifdef USE_PRIVATE_NR2
#include "OxSignCalculatorShmolli.h"
#endif //USE_PRIVATE_NR2

/* *********************** */
/* start point calculators */
/* *********************** */
#include "OxStartPointCalculatorBasic.h"
#ifdef USE_PRIVATE_NR2
#include "OxStartPointCalculatorShmolli.h"
#endif

/* ********* */
/* factories */
/* ********* */
#include "OxFactoryOfCalculators.h"
#include "OxFactoryOfFitters.h"
#include "OxFactoryOfModels.h"
#include "OxFactoryOfSignCalculators.h"
#include "OxFactoryOfStartPointCalculators.h"

/* ********* */
/*    ITK    */
/* ********* */

#ifdef USE_ITK
#include "itkCalculatorT1ImageFilter.h"
#include "itkColorbar2DImageFilter.h"
#include "itkNShmolliSamplesUsedTo123ImageFilter.h"
#include "itkReadFileListFilter.h"
#include "itkSortInvTimesImageFilter.h"

#endif //USE_ITK

namespace Ox {

    /* ************* */
    /*  calculators  */
    /* ************* */
    template class TOMATOLIB_EXPORT Calculator<double>;
    template class TOMATOLIB_EXPORT CalculatorT1Molli<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT CalculatorT1Shmolli<double>;
    template class TOMATOLIB_EXPORT CalculatorT1ShmolliOriginal<double>;
#endif //USE_PRIVATE_NR2

    /* ************* */
    /*    fitters    */
    /* ************* */
    template class TOMATOLIB_EXPORT Fitter<double>;
#ifdef USE_ITK
    template class TOMATOLIB_EXPORT FitterAmoebaVnl<double>;
    template class TOMATOLIB_EXPORT FitterLevenbergMarquardtVnl<double>;
#endif //USE_ITK
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT FitterAmoebaPrivateNr2<double>;
#endif //USE_PRIVATE_NR2
#ifdef USE_LMFIT
    template class TOMATOLIB_EXPORT FitterLevenbergMarquardtLmfit<double>;
#endif // USE_LMFIT

    /* ************* */
    /*   functions   */
    /* ************* */
    template class TOMATOLIB_EXPORT Model<double>;
    template class TOMATOLIB_EXPORT ModelT1ThreeParam<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT ModelT1Shmolli<double>;
#endif //USE_PRIVATE_NR2
#ifdef USE_LMFIT
    template class TOMATOLIB_EXPORT ModelT1AdapterLmfitLeastSquares<double>;
#endif // USE_LMFIT

    /* **************** */
    /* sign calculators */
    /* **************** */
    template class TOMATOLIB_EXPORT SignCalculator<double>;
    template class TOMATOLIB_EXPORT SignCalculatorNoSign<double>;
    template class TOMATOLIB_EXPORT SignCalculatorRealImag<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT SignCalculatorShmolli<double>;
#endif //USE_PRIVATE_NR2

    /* *********************** */
    /* start point calculators */
    /* *********************** */
    template class TOMATOLIB_EXPORT StartPointCalculator<double>;
    template class TOMATOLIB_EXPORT StartPointCalculatorBasic<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT StartPointCalculatorShmolli<double>;
#endif // USE_PRIVATE_NR2

    /* ********* */
    /* factories */
    /* ********* */
    template class TOMATOLIB_EXPORT FactoryOfCalculators<double>;
    template class TOMATOLIB_EXPORT FactoryOfFitters<double>;
    template class TOMATOLIB_EXPORT FactoryOfModels<double>;
    template class TOMATOLIB_EXPORT FactoryOfSignCalculators<double>;
    template class TOMATOLIB_EXPORT FactoryOfStartPointCalculators<double>;

    /* ********* */
    /* options  */
    /* ********* */
    template class TOMATOLIB_EXPORT TomatoOptions<double>;

} //namespace Ox

namespace itk {

    /* ********* */
    /*    ITK    */
    /* ********* */
#ifdef USE_ITK
    template class TOMATOLIB_EXPORT CalculatorT1ImageFilter< Image<double, 3>, Image<double, 3> >;
    template class TOMATOLIB_EXPORT Colorbar2DImageFilter< Image<double, 3> >;
    template class TOMATOLIB_EXPORT NShmolliSamplesUsedTo123ImageFilter< Image<double, 3> >;
    //template class TOMATOLIB_EXPORT ReadDirectoryFilter< Image<double, 3> >;
    template class TOMATOLIB_EXPORT ReadFileListFilter< Image<double, 3> >;
    template class TOMATOLIB_EXPORT SortInvTimesImageFilter< Image<double, 3>, Image<double, 3> >;
#endif //USE_ITK

} //namespace itk

#endif //TOMATO_TOMATOAPI_H
