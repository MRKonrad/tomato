/*!
 * \file OxImageCalculator.h
 * \author Konrad Werys
 * \date 2018/08/07
 */

#ifndef Tomato_OXIMAGECALCULATORT1_H
#define Tomato_OXIMAGECALCULATORT1_H

#include "tomatolib_export.h"
#include "CmakeConfigForTomato.h"
#ifndef CXX_STANDARD_98
#include <thread>
#endif

#include "OxCalculator.h"

namespace Ox {

    template<typename MeasureType>
    class ImageCalculatorT1 {
    public:

        ImageCalculatorT1();
        virtual ~ImageCalculatorT1(){};

        void setUseThreads(bool _useThreads);
        void setNCols(int _nCols);
        void setNRows(int _nRows);
        void setNSamples(int _nSamples);
        void setInvTimes(MeasureType *_invTimes);
        void setImageMag(MeasureType *_imageMag);
        void setImagePha(MeasureType *_imagePha);
        void setImageResults(MeasureType *_imageResults);
        void setCalculatorT1(Calculator<MeasureType> *_calculatorT1);

        MeasureType *getImageResults() const;

        virtual int calculate();
        virtual int calculateOneThread(int posStart, int posStop);

    protected:
        bool _useThreads;
        int _nCols;
        int _nRows;
        int _nSamples;
        MeasureType* _invTimes; // nSamples
        MeasureType* _imageMag; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        MeasureType* _imagePha; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        MeasureType* _imageResults; // nCols * nRows * nDims, address [iCol][iRow][iDim] iDim * (nCols*nRows) + iRow * nCols + iCol
		MeasureType phaseRange;

        Calculator<MeasureType> *_calculatorT1;
    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxImageCalculatorT1.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXIMAGECALCULATORT1_H
