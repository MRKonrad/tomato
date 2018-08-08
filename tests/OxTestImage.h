/*!
 * \file OxTestImage.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef OXShmolli2_OXTESTImage_H
#define OXShmolli2_OXTESTImage_H

#include "OxTestData.h"


namespace Ox {

    template< typename MeasureType >
    class TestImage {

    public:

        TestImage(int nRows, int nCols, std::vector <std::string> filesPaths);
        virtual ~TestImage();

        virtual MeasureType *getInvTimesPtr() ;
        virtual std::vector<MeasureType> getInvTimes() const;

        virtual MeasureType *getImageMagPtr() const;
        virtual MeasureType *getImagePhaPtr() const;
        virtual MeasureType *getImageResultsMolliPtr() const;
        virtual MeasureType *getImageResultsShmolliPtr() const;

        virtual int getNSamples() const { return _nSamples; }

    protected:

        int _nRows;
        int _nCols;
        int _nSamples;

        std::vector<MeasureType> _invTimes;

        MeasureType *_imageMag; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        MeasureType *_imagePha; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol

        MeasureType *_imageResultsMolli; // nRows * nCols * 3
        MeasureType *_imageResultsShmolli; // nRows * nCols * 3

    };
} // namespace Ox

#include "OxTestImage.hxx"

#endif //OXShmolli2_OXTESTImage_H
