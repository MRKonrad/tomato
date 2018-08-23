/*!
 * \file OxShmolli2.h
 * \author Konrad Werys
 * \date 2018/08/14
 */

#ifndef OXSHMOLLI2_OXSHMOLLI2_HXX
#define OXSHMOLLI2_OXSHMOLLI2_HXX

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK

namespace Ox {

    OxShmolli2
    ::OxShmolli2(std::string inputFileName){

        _nSamples = 0;
        _invTimes = 0; // nullptr
        _echoTimes = 0; // nullptr

        _opts = new OxShmolli2Options<InputPixelType>(inputFileName);
        _imageCalculatorItk = CalculatorT1ImageFilterType::New();
        //_sorterMag = SortInvTimesImageFilterType::New();
        //_sorterPha = SortInvTimesImageFilterType::New();
    }

    OxShmolli2
    ::~OxShmolli2(){
        delete _opts;
        delete [] _invTimes;
        delete [] _echoTimes;
    }

    int
    OxShmolli2
    ::readAndSort(){
        ReadFileListFilterType::Pointer readerMag = ReadFileListFilterType::New();
        readerMag->SetFileList(_opts->files_magnitude);
        readerMag->Update();

        ReadFileListFilterType::Pointer readerPha = ReadFileListFilterType::New();
        readerPha->SetFileList(_opts->files_phase);
        readerPha->Update();

        SortInvTimesImageFilterType::Pointer sorterMag = SortInvTimesImageFilterType::New();
        sorterMag->SetInvTimesNonSorted(readerMag->GetInvTimes());
        sorterMag->SetInput(readerMag->GetOutput());
        sorterMag->Update();

        SortInvTimesImageFilterType::Pointer sorterPha = SortInvTimesImageFilterType::New();
        sorterPha->SetInvTimesNonSorted(readerPha->GetInvTimes());
        sorterPha->SetInput(readerPha->GetOutput());
        sorterPha->Update();

        if (sorterMag->GetInvTimesSorted() == sorterPha->GetInvTimesSorted()){
            vnl_vector<InputPixelType > temp = sorterMag->GetInvTimesSorted();
            _nSamples = temp.size();
            _invTimes = new InputPixelType[_nSamples];
            KWUtil::copyArrayToArray(_nSamples, _invTimes, temp.data_block());
        } else {
            throw std::runtime_error("Mag and Pha inv times are not equal");
        }

        _dictionaryInput = readerMag->GetDicomIO()->GetMetaDataDictionary();
        _imageMag = sorterMag->GetOutput();
        _imagePha = sorterPha->GetOutput();

        return 0; // EXIT_SUCCESS
    }

    int
    OxShmolli2
    ::calculate() {

        // alloc and init
        CalculatorT1<InputPixelType> *calculatorT1 = FactoryOfCalculators<InputPixelType>::newByFactory(_opts);
        FunctionsT1<InputPixelType> *functionsT1 = FactoryOfFunctions<InputPixelType>::newByFactory(_opts);
        Fitter<InputPixelType> *fitter = FactoryOfFitters<InputPixelType>::newByFactory(_opts);
        SignCalculator<InputPixelType> *signCalculator = FactoryOfSignCalculators<InputPixelType>::newByFactory(_opts);
        StartPointCalculator<InputPixelType> *startPointCalculator = FactoryOfStartPointCalculators<InputPixelType>::newByFactory(_opts);

        // configure calculator
        calculatorT1->setFunctionsT1(functionsT1);
        calculatorT1->setFitter(fitter);
        calculatorT1->setSignCalculator(signCalculator);
        calculatorT1->setStartPointCalculator(startPointCalculator);
        calculatorT1->setInvTimes(_invTimes);
        calculatorT1->setNSamples(_nSamples);

        // configure calculator itk filter
        //CalculatorT1ImageFilterType::Pointer _imageCalculatorItk = CalculatorT1ImageFilterType::New();
        //_imageCalculatorItk = CalculatorT1ImageFilterType::New();
        _imageCalculatorItk->SetInputMagImage(_imageMag);
        _imageCalculatorItk->SetInputPhaImage(_imagePha);
        if (_opts->number_of_threads > 0)
            _imageCalculatorItk->SetNumberOfThreads(_opts->number_of_threads);
        _imageCalculatorItk->SetCalculator(calculatorT1);

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
        printf("Calculation time: %.4fs.\n", clock.GetTotal());

        delete functionsT1;
        delete fitter;
        delete signCalculator;
        delete startPointCalculator;
        delete calculatorT1;

        return 0; // EXIT_SUCCESS
    }

    int
    OxShmolli2
    ::visualise(){

#ifdef USE_VTK

        // visual presentation of the results
        if (_opts->visualise) {

            // OxColorbarImageFilter
            typedef itk::Colorbar2DImageFilter< OutputImageType > ColorbarImageFilterType;
            ColorbarImageFilterType::Pointer ColorbarFilter = ColorbarImageFilterType::New();
            ColorbarFilter->SetInput(_imageCalculatorItk->GetT1Image());

            QuickView viewer;
            viewer.AddImage(_imageCalculatorItk->GetT1Image(), true, "T1");
            viewer.AddImage(ColorbarFilter->GetOutput(), true, "T1 with colorbar");
            viewer.AddImage(_imageCalculatorItk->GetR2Image(), true, "R2");
            viewer.AddImage(_imageCalculatorItk->GetAImage(), true, "A");
            viewer.AddImage(_imageCalculatorItk->GetBImage(), true, "B");
            viewer.AddImage(_imageCalculatorItk->GetT1starImage(), true, "T1*");
            viewer.AddImage(_imageCalculatorItk->GetChiSqrtImage(), true, "ChiSqrt");
            viewer.AddImage(_imageCalculatorItk->GetSNRImage(), true, "SNR");
            viewer.AddImage(_imageCalculatorItk->GetNShmolliSamplesUsedImage(), true, "Number of Samples used in reconstruction");
            viewer.AddImage(_imageCalculatorItk->GetSD_AImage(), true, "SD A");
            viewer.AddImage(_imageCalculatorItk->GetSD_BImage(), true, "SD B");
            viewer.AddImage(_imageCalculatorItk->GetSD_T1Image(), true, "SD T1");
            //viewer.SetViewPortSize(500);
            viewer.Visualize();
        }

#else // USE_VTK
        printf("Visualisation not possible: the project was configured not to use VTK. Install VTK and reconfigure the project in CMake.");
        //throw itk::ExceptionObject(__FILE__, __LINE__, "Visualisation not possible: the project was configured not to use VTK. Install VTK and reconfigure the project in CMake.");
#endif // USE_VTK

        return 0; // EXIT_SUCCESS
    }

} // namespace Ox

#endif

#endif //OXSHMOLLI2_OXSHMOLLI2_H
