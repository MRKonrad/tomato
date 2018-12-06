/*!
 * \file OxShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "gtest/gtest.h"
#include "OxOriginalShmolliDicomReader.h"
#include "Tomato.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK
#ifdef USE_PRIVATE_NR2

/**
 * for pixel after pixel debugging
 */
TEST(OxCalculator_SameResultsAsDicom, calculate) {

    typedef double TYPE;

    Ox::Tomato<TYPE> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");

    Ox::OriginalShmolliDicomReader<TYPE> originalShmolliDicomReader;
    originalShmolliDicomReader._filesMag = Tomato_object._opts->files_magnitude;
    originalShmolliDicomReader._filesPha = Tomato_object._opts->files_phase;
    originalShmolliDicomReader._filesT1.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_T1MAP_14/IM-0003-0001.dcm");
    originalShmolliDicomReader._filesT1.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_T1MAP_14/IM-0003-0002.dcm");
    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0001.dcm");
    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0002.dcm");
    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0003.dcm");
    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0004.dcm");
    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0005.dcm");
    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0006.dcm");
    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0007.dcm");

    originalShmolliDicomReader.readAndSort();

    int nSamples = originalShmolliDicomReader._nSamples;
    TYPE *invTimes = originalShmolliDicomReader._invTimes;

    Ox::OriginalShmolliDicomReader<TYPE>::Image2dType::IndexType index2d;
    index2d[0] = 41-1;
    index2d[1] = 199-1;

    Ox::OriginalShmolliDicomReader<TYPE>::Image3dType::IndexType index3d;
    index3d[0] = index2d[0];
    index3d[1] = index2d[1];
    index3d[2] = 0;

    TYPE *sigMag = new TYPE[nSamples];
    TYPE *sigPha = new TYPE[nSamples];

    originalShmolliDicomReader.copyFromImage(sigMag, originalShmolliDicomReader._imageMag, index3d);
    originalShmolliDicomReader.copyFromImage(sigPha, originalShmolliDicomReader._imagePha, index3d);

    // init the necessary objects
    Ox::FunctionsT1CalculatorShmolli<TYPE> functionsObject;
    Ox::FitterAmoebaPrivateNr2<TYPE> fitter;
    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    //    fitter.setTrace(true);
    //    fitter.setVerbose(true);

    // configure
    calculator.setFunctionsT1(&functionsObject);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(invTimes);
    calculator.setSigMag(sigMag);
    calculator.setSigPha(sigPha);
    calculator.setMeanCutOff(10);

    //calculator.prepareToCalculate();
    calculator.calculate();

//    std::cout << std::endl ;
//    std::cout << "oryginal/calculated"<< std::endl ;
//    std::cout << originalShmolliDicomReader._imageA->GetPixel(index2d) << " " << calculator.getResults().A << std::endl ;
//    std::cout << originalShmolliDicomReader._imageB->GetPixel(index2d) << " " <<calculator.getResults().B << std::endl ;
//    std::cout << originalShmolliDicomReader._imageT1star->GetPixel(index2d) << " " <<calculator.getResults().T1star << std::endl ;
//
//    TYPE temp1[3] = {120, 28, 50000};
//
//    functionsObject.setNSamples(7);
//    functionsObject.setSignal(sigMag);
//    functionsObject.setInvTimes(invTimes);
//
//    functionsObject.setParameters(temp1);
//
//    functionsObject.disp();
//
//    std::cout << functionsObject.calcCostValue() << std::endl ;
//
//    TYPE temp2[3] = {100,4,4818};
//    functionsObject.setParameters(temp2);
//    std::cout << functionsObject.calcCostValue() << std::endl ;
//
//
//    OxOld::MOLLI_amoeba_fit FitData( 1.e-12, 4000, true);
//    FitData.siMolliN = 7;
//    FitData.sdMolliBestCost = 1e+32;
//    KWUtil::copyArrayToArray(7,FitData.sdMolliSign, sigMag);
//    KWUtil::copyArrayToArray(7,FitData.sdMollitTIs, invTimes);
//    std::cout << FitData.func(temp1) << std::endl;

    EXPECT_DOUBLE_EQ((int)calculator.getResults().A, 350);
    EXPECT_DOUBLE_EQ((int)calculator.getResults().B, 676);
    EXPECT_DOUBLE_EQ((int)calculator.getResults().T1star, 102);

//    originalShmolliDicomReader._visualise = true;
//    originalShmolliDicomReader.visualise();

}

#endif // USE_PRIVATE_NR2
#endif // USE_ITK