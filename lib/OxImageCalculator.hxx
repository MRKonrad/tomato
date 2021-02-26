/*!
 * \file OxImageCalculator.hxx
 * \author Konrad Werys
 * \date 2018/08/08
 */

#ifndef Tomato_OxImageCalculator_HXX
#define Tomato_OxImageCalculator_HXX

namespace Ox {

    template<typename MeasureType>
    ImageCalculator<MeasureType>
    ::ImageCalculator(){
        _useThreads = true;
        _nThreads = 0;
        _nCols = 0;
        _nRows = 0;
        _nSamples = 0;
        _invTimes = 0; // nSamples
        _echoTimes = 0; // nSamples
        _imageMag = 0; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        _imagePha = 0; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        _imageResultsArray = 0; // nCols * nRows * nDims, address [iCol][iRow][iDim] iDim * (nCols*nRows) + iRow * nCols + iCol
        _imageResultsMap = 0; // map of arrays, each size nCols * nRows

        _calculator = 0;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setUseThreads(bool _useThreads) {
        ImageCalculator::_useThreads = _useThreads;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setNCols(int _nCols) {
        ImageCalculator::_nCols = _nCols;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setNRows(int _nRows) {
        ImageCalculator::_nRows = _nRows;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setNSamples(int _nSamples) {
        ImageCalculator::_nSamples = _nSamples;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setInvTimes(const MeasureType *_invTimes) {
        ImageCalculator::_invTimes = _invTimes;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setEchoTimes(const MeasureType *_echoTimes) {
        ImageCalculator::_echoTimes = _echoTimes;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setImageMag(const MeasureType *_imageMag) {
        ImageCalculator::_imageMag = _imageMag;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setImagePha(const MeasureType *_imagePha) {
        ImageCalculator::_imagePha = _imagePha;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setImageResultsArray(MeasureType *_imageResultsArray) {
        ImageCalculator::_imageResultsArray = _imageResultsArray;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setCalculator(Calculator <MeasureType> *_calculator) {
        ImageCalculator::_calculator = _calculator;
    }

    template<typename MeasureType>
    MeasureType *
    ImageCalculator<MeasureType>
    ::getImageResultsArray() const {
        return _imageResultsArray;
    };

    template<typename MeasureType>
    int
    ImageCalculator<MeasureType>
    ::calculate(){

        if (!_useThreads) {
            int posStart = 0;
            int posStop = _nCols * _nRows;
            calculateOneThread(posStart, posStop);
        }
        else {
#ifndef TOMATO_USES_CXX_STANDARD_98

            // get the threads ready
            std::vector<std::thread> threads;
            unsigned concurentThreadsSupported = std::thread::hardware_concurrency();

            if (_nThreads == 0){
                _nThreads = concurentThreadsSupported;
            }

            // calc the range of pixels for each thread
            std::vector<int> limits = KWUtil::bounds<MeasureType>(concurentThreadsSupported, _nCols * _nRows);

            // threaded loop
            for (int i = 0; i < concurentThreadsSupported; ++i){
                threads.push_back(std::thread(&ImageCalculator<MeasureType>::calculateOneThread, this, limits[i], limits[i+1]));
            }

            // finish threads
            for(auto &t : threads){
                t.join();
            }
#else
            throw std::runtime_error("Threads not supported in Tomato with C++98");
#endif
        }
        return 0; // EXIT_SUCCESS
    }

    template<typename MeasureType>
    int
    ImageCalculator<MeasureType>
    ::calculateOneThread(int posStart, int posStop){

        // perform all the calculations on the copies of original objects!!!

        // clone the necessary objects
        Ox::Calculator<MeasureType> *calculator = _calculator->newByCloning();
        Ox::Model<MeasureType> *model = 0;
        if (_calculator->getModel()) {
            model = _calculator->getModel()->newByCloning();
        }
        Ox::Fitter<MeasureType> *fitter = 0;
        if (_calculator->getFitter()) {
            fitter = _calculator->getFitter()->newByCloning();
        }
        Ox::SignCalculator<MeasureType> *signCalculator = 0;
        if (_calculator->getSignCalculator()){
            signCalculator = _calculator->getSignCalculator()->newByCloning();
        }
        Ox::StartPointCalculator<MeasureType> *startPointCalculator = 0;
        if (_calculator->getStartPointCalculator()) {
            startPointCalculator = _calculator->getStartPointCalculator()->newByCloning();
        }

        // configure
        if (model) calculator->setModel(model);
        if (fitter) calculator->setFitter(fitter);
        if (signCalculator) calculator->setSignCalculator(signCalculator);
        if (startPointCalculator) calculator->setStartPointCalculator(startPointCalculator);

        calculator->setNSamples(_nSamples);
        calculator->setInvTimes(_invTimes);
        calculator->setEchoTimes(_echoTimes);

        for (int pos = posStart; pos < posStop; ++pos) {
            // set the data
            MeasureType *sigMag = new MeasureType[_nSamples];
            MeasureType *sigPha = new MeasureType[_nSamples];

            for (int iSample = 0; iSample < _nSamples; ++iSample) {
                sigMag[iSample] = _imageMag[iSample * (_nCols * _nRows) + pos];
                if (_imagePha) sigPha[iSample] = _imagePha[iSample * (_nCols * _nRows) + pos];
            }

            calculator->setSigMag(sigMag);
            if(_imagePha) calculator->setSigPha(sigPha);

            calculator->calculate();

            // store results as array
            if (_imageResultsArray) {
                for (int iDim = 0; iDim < calculator->getNDims(); iDim++){
                    _imageResultsArray[iDim * (_nCols * _nRows) + pos] = calculator->getParametersAfterFitting()[iDim];
                }
            }

            // store results as map
            if (_imageResultsMap) {
                // iterate over each pair in the map
                typename std::map<std::string, MeasureType*>::iterator it;
                for ( it = _imageResultsMap->begin(); it != _imageResultsMap->end(); it++ ){
                    it->second[pos] = calculator->getResults()[it->first];
                }
            }

            delete [] sigMag;
            delete [] sigPha;
        }

        delete calculator;
        delete model;
        delete fitter;
        delete signCalculator;
        delete startPointCalculator;

        return 0; // EXIT_SUCCESS
    }

    template<typename MeasureType>
    void ImageCalculator<MeasureType>::setImageResultsMap(std::map<std::string, MeasureType *> *imageResultsMap) {
        _imageResultsMap = imageResultsMap;
    }

    template<typename MeasureType>
    std::map<std::string, MeasureType *> *ImageCalculator<MeasureType>::getImageResultsMap() const {
        return _imageResultsMap;
    }

    template<typename MeasureType>
    void ImageCalculator<MeasureType>::setNThreads(int nThreads) {
        _nThreads = nThreads;
    }

    template<typename MeasureType>
    bool ImageCalculator<MeasureType>::isUseThreads() const {
        return _useThreads;
    }

    template<typename MeasureType>
    unsigned ImageCalculator<MeasureType>::getNThreads() const {
        return _nThreads;
    }

} // namespace Ox

#endif //Tomato_OxImageCalculator_H
