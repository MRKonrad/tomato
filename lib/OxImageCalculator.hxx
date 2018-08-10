/*!
 * \file OxImageCalculator.hxx
 * \author Konrad Werys
 * \date 2018/08/08
 */

#ifndef OXSHMOLLI2_OXIMAGECALCULATOR_HXX
#define OXSHMOLLI2_OXIMAGECALCULATOR_HXX

namespace Ox {

    template<typename MeasureType>
    ImageCalculator<MeasureType>
    ::ImageCalculator(){
        _nCols = 0;
        _nRows = 0;
        _nSamples = 0;
        _useThreads = true;
        _invTimes = 0; // nSamples
        _imageMag = 0; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        _imagePha = 0; // nCols * nRows * nSamples, address [iCol][iRow][iSam] iSample * (nCols*nRows) + iRow * nCols + iCol
        _imageResults = 0; // nCols * nRows * nDims, address [iCol][iRow][iDim] iDim * (nCols*nRows) + iRow * nCols + iCol

        _calculatorT1 = 0;
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
    ::setInvTimes( MeasureType *_invTimes) {
        ImageCalculator::_invTimes = _invTimes;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setImageMag( MeasureType *_imageMag) {
        ImageCalculator::_imageMag = _imageMag;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setImagePha( MeasureType *_imagePha) {
        ImageCalculator::_imagePha = _imagePha;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setImageResults( MeasureType *_imageResults) {
        ImageCalculator::_imageResults = _imageResults;
    }

    template<typename MeasureType>
    void
    ImageCalculator<MeasureType>
    ::setCalculatorT1(CalculatorT1 <MeasureType> *_calculatorT1) {
        ImageCalculator::_calculatorT1 = _calculatorT1;
    }

    template<typename MeasureType>
    MeasureType *
    ImageCalculator<MeasureType>
    ::getImageResults() const {
        return _imageResults;
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
#ifndef CXX_STANDARD_98

            // get the threads ready
            std::vector<std::thread> threads;
            unsigned concurentThreadsSupported = std::thread::hardware_concurrency();

            std::cout << "Number of threads: " << concurentThreadsSupported << std::endl;

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
            std::cout << "Threads not supported in OxShmolli2 with C++98" << std::endl;
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
        Ox::CalculatorT1<MeasureType> *calculatorT1 = _calculatorT1->newByCloning();
        Ox::FunctionsT1<MeasureType> *functionsObject = _calculatorT1->getFunctionsT1()->newByCloning();
        Ox::Fitter<MeasureType> *fitter = _calculatorT1->getFitter()->newByCloning();
        Ox::SignCalculator<MeasureType> *signCalculator = _calculatorT1->getSignCalculator()->newByCloning();
        Ox::StartPointCalculator<MeasureType> *startPointCalculator = _calculatorT1->getStartPointCalculator()->newByCloning();

        // configure
        calculatorT1->setFunctionsT1(functionsObject);
        calculatorT1->setFitter(fitter);
        calculatorT1->setSignCalculator(signCalculator);
        calculatorT1->setStartPointCalculator(startPointCalculator);
        calculatorT1->setNSamples(_nSamples);
        calculatorT1->setInvTimes(_invTimes);


        for (int pos = posStart; pos < posStop; ++pos) {
            // set the data
            MeasureType *sigMag = new MeasureType[_nSamples];
            MeasureType *sigPha = new MeasureType[_nSamples];

            for (int iSample = 0; iSample < _nSamples; ++iSample) {
                sigMag[iSample] = _imageMag[iSample * (_nCols * _nRows) + pos];
                sigPha[iSample] = _imagePha[iSample * (_nCols * _nRows) + pos];
            }


            calculatorT1->setSigMag(sigMag);
            calculatorT1->setSigPha(sigPha);

            calculatorT1->calculate();

            _imageResults[0 * (_nCols * _nRows) + pos] = calculatorT1->getResults().A;
            _imageResults[1 * (_nCols * _nRows) + pos] = calculatorT1->getResults().B;
            _imageResults[2 * (_nCols * _nRows) + pos] = calculatorT1->getResults().T1star;

            delete [] sigMag;
            delete [] sigPha;
        }

        delete calculatorT1;
        delete functionsObject;
        delete fitter;
        delete signCalculator;
        delete startPointCalculator;

        return 0; // EXIT_SUCCESS
    }

} // namespace Ox

#endif //OXSHMOLLI2_OXIMAGECALCULATOR_H
