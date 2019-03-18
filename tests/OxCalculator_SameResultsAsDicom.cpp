/*!
 * \file OxCalculator_SameResultsAsDicom_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_ITK
#ifdef USE_PRIVATE_NR2

#include "gtest/gtest.h"
#include "OxOriginalShmolliDicomReader.h"
#include "Tomato.h"

///**
// * for pixel after pixel debugging
// * TODO: difference of 1, correct it!
// */
//TEST(OxCalculator_SameResultsAsDicom, compare_one_pixel) {
//
//    typedef double TYPE;
//
//    Ox::Tomato<TYPE> Tomato_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");
//
//    Ox::OriginalShmolliDicomReader<TYPE> originalShmolliDicomReader;
//    originalShmolliDicomReader._filesMag = Tomato_object._opts->files_magnitude;
//    originalShmolliDicomReader._filesPha = Tomato_object._opts->files_phase;
//    originalShmolliDicomReader._filesT1.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_T1MAP_14/IM-0003-0001.dcm");
//    originalShmolliDicomReader._filesT1.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_T1MAP_14/IM-0003-0002.dcm");
//    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0001.dcm");
//    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0002.dcm");
//    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0003.dcm");
//    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0004.dcm");
//    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0005.dcm");
//    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0006.dcm");
//    originalShmolliDicomReader._filesFitparams.push_back("testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_FITPARAMS_15/IM-0004-0007.dcm");
//
//    originalShmolliDicomReader.readAndSort();
//
//    int nSamples = originalShmolliDicomReader._nSamples;
//    TYPE *invTimes = originalShmolliDicomReader._invTimes;
//
//    Ox::OriginalShmolliDicomReader<TYPE>::Image2dType::IndexType index2d;
////    index2d[0] = 41-1;
////    index2d[1] = 199-1;
//
//    index2d[0] = 87;
//    index2d[1] = 103;
//
//    Ox::OriginalShmolliDicomReader<TYPE>::Image3dType::IndexType index3d;
//    index3d[0] = index2d[0];
//    index3d[1] = index2d[1];
//    index3d[2] = 0;
//
//    TYPE *sigMag = new TYPE[nSamples];
//    TYPE *sigPha = new TYPE[nSamples];
//
//    originalShmolliDicomReader.copyFromImage(sigMag, originalShmolliDicomReader._imageMag, index3d);
//    originalShmolliDicomReader.copyFromImage(sigPha, originalShmolliDicomReader._imagePha, index3d);
//
//    // init the necessary objects
//    Ox::FunctionsT1Shmolli<TYPE> functionsObject;
//    Ox::FitterAmoebaPrivateNr2<TYPE> fitter;
//    Ox::SignCalculatorShmolli<TYPE> signCalculator;
//    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
//    Ox::CalculatorT1Shmolli<TYPE> calculatorT1Shmolli;
//
//    //fitter.setTrace(true);
//    //fitter.setVerbose(true);
//
//    // configure
//    calculatorT1Shmolli.setFunctionsT1(&functionsObject);
//    calculatorT1Shmolli.setFitter(&fitter);
//    calculatorT1Shmolli.setSignCalculator(&signCalculator);
//    calculatorT1Shmolli.setStartPointCalculator(&startPointCalculator);
//
//    // set the data
//    calculatorT1Shmolli.setNSamples(nSamples);
//    calculatorT1Shmolli.setInvTimes(invTimes);
//    calculatorT1Shmolli.setSigMag(sigMag);
//    calculatorT1Shmolli.setSigPha(sigPha);
//    calculatorT1Shmolli.setMeanCutOff(10);
//
//    //fitter.setTrace(true);
//    //fitter.setVerbose(true);
//    std::cout << std::endl;
//    std::cout << "calculatorT1Shmolli: " << std::endl;
//    calculatorT1Shmolli.calculate();
//
//    // init the necessary objects
//    Ox::CalculatorT1ShmolliOriginal<TYPE> calculatorT1ShmolliOriginal;
//
//    // set the data
//    calculatorT1ShmolliOriginal.setNSamples(nSamples);
//    calculatorT1ShmolliOriginal.setInvTimes(invTimes);
//    calculatorT1ShmolliOriginal.setSigMag(sigMag);
//    calculatorT1ShmolliOriginal.setSigPha(sigPha);
//    calculatorT1ShmolliOriginal.setMeanCutOff(10);
//
//    std::cout << std::endl;
//    std::cout << "calculatorT1ShmolliOriginal: " << std::endl;
//    calculatorT1ShmolliOriginal.calculate();
//
//    Ox::FunctionsT1Shmolli<TYPE> functionsObject2;
//    TYPE params[3] = {19.00019,	59.03927, 96.74255};
//    TYPE signal5[5] = {-2, 19, 19, 19, 19 };
//    TYPE invTimes5[5] = {100, 1100, 2100, 3100, 4100 };
//    //TYPE invTimes5[5] = {0, 0, 0, 0, 0 };
//    functionsObject2.setInvTimes(invTimes5);
//    functionsObject2.setSignal(signal5);
//    functionsObject2.setNSamples(5);
//
//    std::cout << std::endl;
//    for (int i = 0; i < 5; ++i){
//        TYPE temp1 = functionsObject2.calcModelValue(params, invTimes5[i]);
//        std::cout << temp1 << std::endl;
//    }
//
//    for (int i = 0; i < 5; ++i){
//        TYPE temp1 = functionsObject2.calcModelValue(params, invTimes5[i]);
//        std::cout << temp1 - signal5[i] << std::endl;
//    }
//
//    TYPE temp2 = functionsObject2.calcCostValue(params);
//    std::cout << temp2 << std::endl;
//
//    functionsObject2.disp();
//
//    OxOld::MOLLI_amoeba_fit FitData( 1.e-12, 4000, true);
//    //FitData.sdMolliSign = calculatorT1Shmolli.getSignal();
//    //TYPE temp2 = FitData.func(params);
//
//    //std::cout << temp2 << std::endl;
////    calculatorT1ShmolliOriginal.
//
//    TYPE A = originalShmolliDicomReader._imageA->GetPixel(index2d);
//    TYPE B = originalShmolliDicomReader._imageB->GetPixel(index2d);
//    TYPE T1star = originalShmolliDicomReader._imageT1star->GetPixel(index2d);
//    TYPE T1 = originalShmolliDicomReader._imageT1color->GetPixel(index2d);
//
//    if ((int)calculatorT1Shmolli.getResults().A != A
//        || (int)calculatorT1Shmolli.getResults().B != B
//        || (int)calculatorT1Shmolli.getResults().T1star != T1star
//        || (int)calculatorT1Shmolli.getResults().T1 != T1) {
//
//        printf("\nT1s calc new = %f\nT1s calc old = %f\nT1s dicom = %f", calculatorT1Shmolli.getResults().T1star, calculatorT1ShmolliOriginal.getResults().T1star, T1star);
//        printf("\nT1 calc new = %f\nT1 calc old = %f\nT1 dicom = %f", calculatorT1Shmolli.getResults().T1, calculatorT1ShmolliOriginal.getResults().T1, T1);
//
//        KWUtil::printArray(nSamples, sigMag, (char *)"\nsigMag: ");
//        KWUtil::printArray(nSamples, sigPha, (char *)"\nsigPha: ");
//        KWUtil::printArray(nSamples, invTimes, (char *)"\ninvTimes: ");
//        printf("\n");
//    }
//
//    EXPECT_DOUBLE_EQ((int)calculatorT1Shmolli.getResults().A, A);
//    EXPECT_DOUBLE_EQ((int)calculatorT1Shmolli.getResults().B, B);
//    EXPECT_DOUBLE_EQ((int)calculatorT1Shmolli.getResults().T1star, T1star);
//    EXPECT_DOUBLE_EQ((int)calculatorT1Shmolli.getResults().T1, T1);
//
//    delete [] sigMag;
//    delete [] sigPha;
//
////    originalShmolliDicomReader._visualise = true;
////    originalShmolliDicomReader.visualise();
//
//}

#endif // USE_PRIVATE_NR2
#endif // USE_ITK