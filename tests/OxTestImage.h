/*!
 * \file OxTestImage.h
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef Tomato_OXTESTImage_H
#define Tomato_OXTESTImage_H

#include "CmakeConfigForTomato.h"

#ifdef USE_YAML

#include "OxTestData.h"


namespace Ox {

    template< typename MeasureType >
    class TestImage {

    public:

        TestImage(int nCols, int nRows, std::vector <std::string> filesPaths, std::vector<int> invTimesOrder);
        TestImage(int nCols, int nRows, std::vector <std::string> filesPaths);
        virtual ~TestImage();

        int init(int nCols, int nRows, std::vector <std::string> filesPaths, std::vector<int> invTimesOrder);

        virtual MeasureType *getInvTimesPtr() ;
        virtual std::vector<MeasureType> getInvTimes() const;

        virtual MeasureType *getImageMagPtr() const;
        virtual MeasureType *getImagePhaPtr() const;
        virtual MeasureType *getImageResultsMolliPtr() const;
        virtual MeasureType *getImageResultsShmolliPtr() const;

        virtual int getNCols() const { return _nCols; }
        virtual int getNRows() const { return _nRows; }
        virtual int getNSamples() const { return _nSamples; }

    protected:

        int _nCols;
        int _nRows;
        int _nSamples;

        std::vector<MeasureType> _invTimes;
        std::vector<int> _invTimesOrder;

        MeasureType *_imageMag; // nRows * nCols * nSamples, address [iRow][iCol][iSam] iSample * (nRows*nCols) + iCol * nRows + iRow
        MeasureType *_imagePha; // nRows * nCols * nSamples, address [iRow][iCol][iSam] iSample * (nRows*nCols) + iCol * nRows + iRow

        MeasureType *_imageResultsMolli; // nCols * nRows * 3
        MeasureType *_imageResultsShmolli; // nCols * nRows * 3

    };
} // namespace Ox

#include "OxTestImage.hxx"

#endif // USE_YAML

#endif //Tomato_OXTESTImage_H
