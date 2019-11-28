/*!
 * \file Tomato.h
 * \author Konrad Werys
 * \date 2018/08/14
 */

#ifndef Tomato_Tomato_HXX
#define Tomato_Tomato_HXX

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK

namespace Ox {

    template< typename MeasureType >
    Tomato<MeasureType>
    ::Tomato(std::string inputFileName){

        _nSamples = 0;
        _invTimes = 0; // nullptr
        _echoTimes = 0; // nullptr

        _opts = new TomatoOptions<InputPixelType>(inputFileName);
        _imageCalculatorItk = CalculatorT1ImageFilterType::New();
        //_sorterMag = SortInvTimesImageFilterType::New();
        //_sorterPha = SortInvTimesImageFilterType::New();
    }

    template< typename MeasureType >
    Tomato<MeasureType>
    ::~Tomato(){
        delete _opts;
        delete [] _invTimes;
        delete [] _echoTimes;
    }

    template< typename MeasureType >
    int
    Tomato<MeasureType>
    ::readAndSort(){

        typename ReadFileListFilterType::Pointer readerMag = ReadFileListFilterType::New();
        readerMag->SetFileList(_opts->files_magnitude);
        readerMag->SetDirName(_opts->dir_magnitude);
        readerMag->Update();

        // have we read magnitudes?
        if (readerMag->GetInvTimes().size() == 0){
            throw std::runtime_error("\nNo magnitude images read, check the paths!");
        }

        typename SortInvTimesImageFilterType::Pointer sorterMag = SortInvTimesImageFilterType::New();
        sorterMag->SetInvTimesNonSorted(readerMag->GetInvTimes());
        sorterMag->SetEchoTimesNonSorted(readerMag->GetEchoTimes());
        sorterMag->SetInput(readerMag->GetOutput());
        if (_opts->parameter_type == T1) {
            sorterMag->SortByInvTimes();
        } else if (_opts->parameter_type == T2){
            sorterMag->SortByEchoTimes();
        }
        sorterMag->Update();

        typename ReadFileListFilterType::Pointer readerPha = ReadFileListFilterType::New();
        readerPha->SetFileList(_opts->files_phase);
        readerPha->SetDirName(_opts->dir_magnitude);
        readerPha->Update();

        // is phase empty?
        if (readerPha->GetInvTimes().size() == 0) {
            if (_opts->sign_calc_method != NoSign){
                std::cerr << "\nNo phase images read, setting the sign_calc_method to NoSign" << std::endl;
                _opts->sign_calc_method = NoSign;
            }
        }

        typename SortInvTimesImageFilterType::Pointer sorterPha = SortInvTimesImageFilterType::New();
        if (_opts->sign_calc_method != NoSign) {
            sorterPha->SetInvTimesNonSorted(readerPha->GetInvTimes());
            sorterPha->SetEchoTimesNonSorted(readerMag->GetEchoTimes());
            sorterPha->SetInput(readerPha->GetOutput());
            if (_opts->parameter_type == T1) {
                sorterPha->SortByInvTimes();
            } else if (_opts->parameter_type == T2){
                sorterPha->SortByEchoTimes();
            }
            sorterPha->Update();
        }

        // are the inversion times in magnitude and phase series equal?
        if (_opts->sign_calc_method != NoSign) {
            if (sorterMag->GetInvTimesSorted() != sorterPha->GetInvTimesSorted()) {
                throw std::runtime_error("\nMag and Pha inv times are not equal");
            }
        }

        vnl_vector<InputPixelType > tempInv = sorterMag->GetInvTimesSorted();
        _nSamples = (int)tempInv.size();
        _invTimes = new InputPixelType[_nSamples];
        KWUtil::copyArrayToArray(_nSamples, _invTimes, tempInv.data_block());

        vnl_vector<InputPixelType > tempEcho = sorterMag->GetEchoTimesSorted();
        _nSamples = (int)tempEcho.size();
        _echoTimes = new InputPixelType[_nSamples];
        KWUtil::copyArrayToArray(_nSamples, _echoTimes, tempEcho.data_block());

        _dictionaryInput = readerMag->GetDicomIO()->GetMetaDataDictionary();
        _imageMag = sorterMag->GetOutput();
        if (_opts->sign_calc_method != NoSign) {
            _imagePha = sorterPha->GetOutput();
        }

        return 0; // EXIT_SUCCESS
    }

    template< typename MeasureType >
    int
    Tomato<MeasureType>
    ::calculate() {

        // alloc and init
        Calculator<InputPixelType> *calculator = FactoryOfCalculators<InputPixelType>::newByFactory(_opts);
        Model<InputPixelType> *model = FactoryOfModels<InputPixelType>::newByFactory(_opts);
        Fitter<InputPixelType> *fitter = FactoryOfFitters<InputPixelType>::newByFactory(_opts);
        SignCalculator<InputPixelType> *signCalculator = FactoryOfSignCalculators<InputPixelType>::newByFactory(_opts);
        StartPointCalculator<InputPixelType> *startPointCalculator = FactoryOfStartPointCalculators<InputPixelType>::newByFactory(_opts);

        // configure calculator
        calculator->setModel(model);
        calculator->setFitter(fitter);
        calculator->setSignCalculator(signCalculator);
        calculator->setStartPointCalculator(startPointCalculator);
        calculator->setInvTimes(_invTimes);
        calculator->setEchoTimes(_echoTimes);
        if (_opts->noise.size() > 0){
            calculator->setNoise(&(_opts->noise)[0]);
        }
        calculator->setNSamples(_nSamples);

        // configure calculator itk filter
        //CalculatorT1ImageFilterType::Pointer _imageCalculatorItk = CalculatorT1ImageFilterType::New();
        //_imageCalculatorItk = CalculatorT1ImageFilterType::New();
        _imageCalculatorItk->SetInputMagImage(_imageMag);
        _imageCalculatorItk->SetInputPhaImage(_imagePha);
        if (_opts->number_of_threads > 0)
            _imageCalculatorItk->SetNumberOfThreads(_opts->number_of_threads);
        _imageCalculatorItk->SetCalculator(calculator);

        // calculations
        itk::TimeProbe clock;
        clock.Start();
        try {
            _imageCalculatorItk->Update();
        } catch( itk::ExceptionObject & err ) {
            std::cerr << "ExceptionObject caught !" << std::endl;
            std::cerr << err << std::endl;
        }
        clock.Stop();
        _opts->calculation_time = clock.GetTotal();
        printf("Calculation time: %.4fs.\n", clock.GetTotal());

        delete model;
        delete fitter;
        delete signCalculator;
        delete startPointCalculator;
        delete calculator;

        return 0; // EXIT_SUCCESS
    }

//    template< typename MeasureType >
//    int
//    Tomato<MeasureType>
//    ::calculateOnePixel(TomatoOptions<InputPixelType> opts){
//
//        // alloc and init
//        Calculator<InputPixelType> *calculator = FactoryOfCalculators<InputPixelType>::newByFactory(opts);
//        Model<InputPixelType> *model = FactoryOfModels<InputPixelType>::newByFactory(opts);
//        Fitter<InputPixelType> *fitter = FactoryOfFitters<InputPixelType>::newByFactory(opts);
//        SignCalculator<InputPixelType> *signCalculator = FactoryOfSignCalculators<InputPixelType>::newByFactory(opts);
//        StartPointCalculator<InputPixelType> *startPointCalculator = FactoryOfStartPointCalculators<InputPixelType>::newByFactory(opts);
//
//        if (opts->signal_magnitude.size() > 0){
//            calculator->setNSamples(opts->signal_magnitude.size());
//            calculator->setSigMag(&(opts->signal_magnitude)[0]);
//        } else {
//            throw std::runtime_error("\nNo magnitude signal, I cannot calculate anything");
//        }
//
//        if (opts->signal_phase.size() > 0){
//            calculator->setSigMag(&(opts->signal_phase)[0]);
//        }
//
//        if (opts->noise.size() > 0){
//            calculator->setNoise(&(opts->noise)[0]);
//        }
//
//        if (opts->inversion_times.size() > 0){
//            calculator->setNoise(&(opts->inversion_times)[0]);
//        }
//
//        if (opts->echo_times.size() > 0){
//            calculator->setNoise(&(opts->echo_times)[0]);
//        }
//
//        // configure calculator
//        calculator->setModel(model);
//        calculator->setFitter(fitter);
//        calculator->setSignCalculator(signCalculator);
//        calculator->setStartPointCalculator(startPointCalculator);
//
//        calculator->calculate();
//
//        std::map <std::string, MeasureType> results = calculator->getResults();
//
//        std::cout << "Results: \n";
//        for(typename std::map<std::string, MeasureType >::const_iterator it = results.begin(); it != results.end(); ++it){
//            std::cout << it->first << " " << it->second << "\n";
//        }
//
//        delete model;
//        delete fitter;
//        delete signCalculator;
//        delete startPointCalculator;
//        delete calculator;
//
//        return 0; // EXIT_SUCCESS
//    }

    template< typename MeasureType >
    int
    Tomato<MeasureType>
    ::exportToDicom(){
        if (_opts->parameter_type == Ox::T1){
            return exportT1ToDicom();
        }
        else if (_opts->parameter_type == Ox::T2){
            return exportT2ToDicom();
        }
        else {
            std::cerr << "Tomato::exportToDicom: Export has not been implemented" << std::endl;
            return 0; // EXIT_FAILURE
        }
    }

    template< typename IType, typename EType >
    class CastPixelAccessor
    {
    public:

        typedef IType InternalType;
        typedef EType ExternalType;

        static void Set(InternalType & output, const ExternalType & input)
        {
            output = static_cast<InternalType>( input );
        }

        static ExternalType Get( const InternalType & input )
        {
            return static_cast<ExternalType>( input );
        }
    };

} // namespace Ox

#endif

#endif //Tomato_Tomato_H
