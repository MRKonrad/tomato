/*!
 * \file OxTestImage.hxx
 * \author Konrad Werys
 * \date 2018/08/07
 */

#ifndef OXShmolli2_OXTESTImage_HXX
#define OXShmolli2_OXTESTImage_HXX


namespace Ox {

    template< typename MeasureType >
    TestImage<MeasureType>
    ::TestImage(int nCols, int nRows, std::vector <std::string> filesPaths) {
        _nRows = nRows;
        _nCols = nCols;
        int nTissues = filesPaths.size();
        std::vector< TestData <MeasureType> > TestDataVector; // vector with TestData objects

        // I do want to have less 'tissues' (TestData objects) than columns, just for the sake of simplicity
        if (nTissues > _nCols){
            std::cerr << "Give me more nCols or less tissues" << std::endl;
            throw std::exception();
        }

        // populate TestDataVector
        for (int i = 0; i < nTissues; ++i){
            TestDataVector.push_back(TestData<MeasureType>((char*)filesPaths.at(i).c_str()));
        }

        // check if invTimes are equal
        for (int i = 1; i < nTissues; ++i){
            if(!(TestDataVector.at(i).getInvTimes() == TestDataVector.at(i-1).getInvTimes())){
                std::cerr << "InvTimes are different" << std::endl;
                throw std::exception();
            }
        }
        _invTimes = TestDataVector.at(0).getInvTimes();
        _nSamples = _invTimes.size();

        // allocate memory
        _imageMag = new MeasureType[_nRows*_nCols*_nSamples];
        _imagePha = new MeasureType[_nRows*_nCols*_nSamples];
        _imageResultsMolli = new MeasureType[_nCols*_nRows*3];
        _imageResultsShmolli = new MeasureType[_nCols*_nRows*3];

        // how to divide the memory?
        std::vector<int> ranges = KWUtil::bounds<int>(nTissues, _nCols);

//        std::cout << std::endl;
//        std::cout << std::endl;
//        for (int i = 0; i < ranges.size(); ++i ){
//            std::cout << ranges[i] << std::endl;
//        }

        // fill memory
        for (int iSample = 0; iSample < _nSamples; ++iSample){
            for (int iRow = 0; iRow < _nRows; ++iRow) {
                for (int iTissue = 0; iTissue < ranges.size()-1; ++iTissue) {
                    for (int iCol = ranges[iTissue]; iCol < ranges[iTissue+1]; ++iCol) {
                        _imageMag[iSample * (nCols * nRows) + iRow * nCols + iCol] = TestDataVector.at(iTissue).getSignalMagPtr()[iSample];
                        _imagePha[iSample * (nCols * nRows) + iRow * nCols + iCol] = TestDataVector.at(iTissue).getSignalPhaPtr()[iSample];
                    }
                }
            }
        }
        for (int iDim = 0; iDim < 3; ++iDim){
            for (int iRow = 0; iRow < _nRows; ++iRow) {
                for (int iTissue = 0; iTissue < ranges.size()-1; ++iTissue) {
                    for (int iCol = ranges[iTissue]; iCol < ranges[iTissue+1]; ++iCol) {
                        _imageResultsMolli[iDim * (nCols * nRows) + iRow * nCols + iCol] = TestDataVector.at(iTissue).getResultsMolliPtr()[iDim];
                        _imageResultsShmolli[iDim * (nCols * nRows) + iRow * nCols + iCol] = TestDataVector.at(iTissue).getResultsShmolliPtr()[iDim];
                    }
                }
            }
        }

    }

    template< typename MeasureType >
    TestImage<MeasureType>
    ::~TestImage(){
        delete [] _imageMag;
        delete [] _imagePha;
        delete [] _imageResultsMolli;
        delete [] _imageResultsShmolli;

    }
    template< typename MeasureType >
    std::vector<MeasureType>
    TestImage<MeasureType>
    ::getInvTimes() const {
        return _invTimes;
    }

    template< typename MeasureType >
    MeasureType *
    TestImage<MeasureType>
    ::getInvTimesPtr()  {
        return &_invTimes.at(0);
    }


    template< typename MeasureType >
    MeasureType *
    TestImage<MeasureType>
    ::getImageMagPtr() const {
        return _imageMag;
    }

    template< typename MeasureType >
    MeasureType *
    TestImage<MeasureType>
    ::getImagePhaPtr() const {
        return _imagePha;
    }

    template< typename MeasureType >
    MeasureType *
    TestImage<MeasureType>
    ::getImageResultsMolliPtr() const {
        return _imageResultsMolli;
    }

    template< typename MeasureType >
    MeasureType *
    TestImage<MeasureType>
    ::getImageResultsShmolliPtr() const {
        return _imageResultsShmolli;
    }

} // namespace Ox



#endif //OXShmolli2_OXTESTImage_HXX
