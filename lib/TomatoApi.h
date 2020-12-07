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
#include "OxCalculatorT1WithSignCheck.h"
#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1Shmolli.h"
#include "OxCalculatorT1ShmolliOriginal.h"
#include "OxAmoebaSKPold.h"
#include "OxCalculatorT2.h"
#include "OxCalculatorT2Linear.h"
#include "OxCalculatorT2Truncation.h"
#include "OxCalculatorT2TruncationNoise.h"
#endif //USE_PRIVATE_NR2

/* ************* */
/*    fitters    */
/* ************* */
#include "OxFitter.h"
#ifdef USE_VNL
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#endif // USE_VNL
#ifdef USE_PRIVATE_NR2
#include "OxFitterAmoebaPrivateNr2.h"
#endif //USE_PRIVATE_NR2
#ifdef USE_LMFIT
#include "OxFitterLevenbergMarquardtLmfit.h"
#endif // USE_LMFIT

/* ********** */
/*   models   */
/* ********** */
#include "OxModel.h"
#include "OxModelT1TwoParam.h"
#include "OxModelT1ThreeParam.h"
#ifdef USE_PRIVATE_NR2
#include "OxModelT1Shmolli.h"
#include "OxModelT2OneParam.h"
#include "OxModelT2TwoParam.h"
#include "OxModelT2TwoParamScale.h"
#include "OxModelT2ThreeParam.h"
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

/* ******* */
/* others  */
/* ******* */
#include "OxImageCalculator.h"
#include "TomatoOnePixel.h"
#ifdef USE_YAML
#include "TomatoParser.h"
#endif // USE_YAML

/* ********* */
/*    APP    */
/* ********* */
#ifdef USE_ITK
#include "itkCalculatorT1ImageFilter.h"
#include "itkColorbar2DImageFilter.h"
#include "itkNShmolliSamplesUsedTo123ImageFilter.h"
#include "itkReadFileListFilter.h"
#include "itkSortInvTimesImageFilter.h"
#include "Tomato.h"
#include "OxOriginalShmolliDicomReader.h"
#endif //USE_ITK

namespace Ox {

    /* ************* */
    /*  calculators  */
    /* ************* */
    template class TOMATOLIB_EXPORT Calculator<double>;
    template class TOMATOLIB_EXPORT CalculatorT1WithSignCheck<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT CalculatorT1Shmolli<double>;
    template class TOMATOLIB_EXPORT CalculatorT1ShmolliOriginal<double>;
    template class TOMATOLIB_EXPORT CalculatorT2<double>;
    template class TOMATOLIB_EXPORT CalculatorT2Linear<double>;
    template class TOMATOLIB_EXPORT CalculatorT2Truncation<double>;
    template class TOMATOLIB_EXPORT CalculatorT2TruncationNoise<double>;
#endif //USE_PRIVATE_NR2

    /* ************* */
    /*    fitters    */
    /* ************* */
    template class TOMATOLIB_EXPORT Fitter<double>;
#ifdef USE_VNL
    template class TOMATOLIB_EXPORT FitterAmoebaVnl<double>;
    template class TOMATOLIB_EXPORT FitterLevenbergMarquardtVnl<double>;
#endif //USE_VNL
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT FitterAmoebaPrivateNr2<double>;
#endif //USE_PRIVATE_NR2
#ifdef USE_LMFIT
    template class TOMATOLIB_EXPORT FitterLevenbergMarquardtLmfit<double>;
#endif // USE_LMFIT

    /* ********** */
    /*   models   */
    /* ********** */
    template class TOMATOLIB_EXPORT Model<double>;
    template class TOMATOLIB_EXPORT ModelT1TwoParam<double>;
    template class TOMATOLIB_EXPORT ModelT1ThreeParam<double>;
#ifdef USE_PRIVATE_NR2
    template class TOMATOLIB_EXPORT ModelT1Shmolli<double>;
    template class TOMATOLIB_EXPORT ModelT2OneParam<double>;
    template class TOMATOLIB_EXPORT ModelT2TwoParam<double>;
    template class TOMATOLIB_EXPORT ModelT2TwoParamScale<double>;
    template class TOMATOLIB_EXPORT ModelT2ThreeParam<double>;
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

    /* ******* */
    /* others  */
    /* ******* */
    template std::map<std::string, double> TOMATOLIB_EXPORT calculateOnePixel(TomatoOptions<double> opts);
    template class TOMATOLIB_EXPORT ImageCalculator<double>;
#ifdef USE_YAML
    template class TOMATOLIB_EXPORT TomatoParser<double>;
#endif // USE_YAML

} //namespace Ox

    /* ********* */
    /*    APP    */
    /* ********* */
#ifdef USE_ITK
namespace itk {
    template class TOMATOLIB_EXPORT CalculatorT1ImageFilter< Image<double, 3>, Image<double, 2>, Image<double, 3> >;
    template class TOMATOLIB_EXPORT CalculatorT1ImageFilter< Image<double, 3>, Image<double, 3> >;
    template class TOMATOLIB_EXPORT Colorbar2DImageFilter< Image<double, 2> >;
    template class TOMATOLIB_EXPORT Colorbar2DImageFilter< Image<double, 3> >;
    template class TOMATOLIB_EXPORT NShmolliSamplesUsedTo123ImageFilter< Image<double, 2> >;
    template class TOMATOLIB_EXPORT NShmolliSamplesUsedTo123ImageFilter< Image<double, 3> >;
    template class TOMATOLIB_EXPORT ReadFileListFilter< Image<double, 3> >;
    template class TOMATOLIB_EXPORT SortInvTimesImageFilter< Image<double, 3>, Image<double, 3> >;
} //namespace itk

namespace Ox {
    template class TOMATOLIB_EXPORT Tomato<double>;
    template class TOMATOLIB_EXPORT OriginalShmolliDicomReader<double>;
}
#endif //USE_ITK



#endif //TOMATO_TOMATOAPI_H
