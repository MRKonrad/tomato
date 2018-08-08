/*!
 * \file OxImageCalculator.h
 * \author Konrad Werys
 * \date 2018/08/07
 */

#ifndef OXSHMOLLI2_OXIMAGECALCULATOR_H
#define OXSHMOLLI2_OXIMAGECALCULATOR_H

#include "CmakeConfigForOxShmolli2.h"
#ifndef CXX_STANDARD_98
#include <thread>
#endif

#include "OxCalculatorT1.h"

namespace Ox {

    template<typename MeasureType>
    class ImageCalculator {
    public:

        ImageCalculator();
        virtual ~ImageCalculator(){};

        void setUseThreads(bool _useThreads);
        void setNCols(int _nCols);
        void setNRows(int _nRows);
        void setNSamples(int _nSamples);
        void setInvTimes(MeasureType *_invTimes);
        void setImageMag(MeasureType *_imageMag);
        void setImagePha(MeasureType *_imagePha);
        void setImageResults(MeasureType *_imageResults);
        void setCalculatorT1(CalculatorT1<MeasureType> *_calculatorT1);

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

        CalculatorT1<MeasureType> *_calculatorT1;
    };

} // namespace Ox

#include "OxImageCalculator.hxx"

#endif //OXSHMOLLI2_OXIMAGECALCULATOR_H
