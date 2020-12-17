/*!
 * \file OxImageCalculator.h
 * \author Konrad Werys
 * \date 2018/08/07
 */

#ifndef Tomato_OXIMAGECALCULATOR_H
#define Tomato_OXIMAGECALCULATOR_H

#include "tomatolib_export.h"
#include "CmakeConfigForTomato.h"
#ifndef TOMATO_USES_CXX_STANDARD_98
#include <thread>
#endif

#include "OxCalculator.h"
#include <map>

namespace Ox {

    template<typename MeasureType>
    class ImageCalculator {
    public:

        ImageCalculator();
        virtual ~ImageCalculator(){};

        // setters
        void setUseThreads(bool _useThreads);
        void setNThreads(int nThreads);
        void setNCols(int _nCols);
        void setNRows(int _nRows);
        void setNSamples(int _nSamples);
        void setInvTimes(const MeasureType *_invTimes);
        void setEchoTimes(const MeasureType *_echoTimes);
        void setImageMag(const MeasureType *_imageMag);
        void setImagePha(const MeasureType *_imagePha);
        void setImageResultsArray(MeasureType *_imageResultsArray);
        void setImageResultsMap(std::map<std::string, MeasureType *> *imageResultsMap);

        void setCalculator(Calculator<MeasureType> *_calculator);

        // getters
        MeasureType *getImageResultsArray() const;
        std::map<std::string, MeasureType *> *getImageResultsMap() const;

        bool isUseThreads() const;
        unsigned getNThreads() const;

        // calculators
        virtual int calculate();
        virtual int calculateOneThread(int posStart, int posStop);

    protected:
        // input
        Calculator<MeasureType> *_calculator;

        bool _useThreads;
        unsigned _nThreads;
        int _nCols;
        int _nRows;
        int _nSamples;
        const MeasureType* _invTimes; // nSamples
        const MeasureType* _echoTimes; // nSamples
        const MeasureType* _imageMag; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        const MeasureType* _imagePha; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol

        // output
        MeasureType* _imageResultsArray; // nCols * nRows * nDims, address [iCol][iRow][iDim] iDim * (nCols*nRows) + iRow * nCols + iCol
        std::map<std::string, MeasureType*> * _imageResultsMap; // map of arrays, each size nCols * nRows


    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxImageCalculator.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXIMAGECALCULATOR_H
