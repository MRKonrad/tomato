/*!
 * \file OxShmolli_test.cpp
 * \author Konrad Werys
 * \date 2018/08/20
 */

#include "gtest/gtest.h"
#include "OxOriginalShmolliDicomReader.h"
#include "OxShmolli2.h"

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_ITK
#ifdef USE_PRIVATE_NR2

TEST(OxCalculator_SameResultsAsDicom, readAndSort) {

    typedef double TYPE;

    Ox::OxShmolli2<TYPE> OxShmolli2_object("testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml");

    Ox::OriginalShmolliDicomReader<TYPE> originalShmolliDicomReader;
    originalShmolliDicomReader._filesMag = OxShmolli2_object._opts->files_magnitude;
    originalShmolliDicomReader._filesPha = OxShmolli2_object._opts->files_phase;
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
    index2d[0] = 309-1;
    index2d[1] = 103-1;

    Ox::OriginalShmolliDicomReader<TYPE>::Image3dType::IndexType index3d;
    index3d[0] = index2d[0];
    index3d[1] = index2d[1];
    index3d[2] = 0;

    TYPE *sigMag = new TYPE[nSamples];
    TYPE *sigPha = new TYPE[nSamples];

    originalShmolliDicomReader.copyFromImage(sigMag, originalShmolliDicomReader._imageMag, index3d);
    originalShmolliDicomReader.copyFromImage(sigPha, originalShmolliDicomReader._imagePha, index3d);

    KWUtil::printArray(nSamples, sigMag, "\n");
    KWUtil::printArray(nSamples, sigPha, "\n");
    KWUtil::printArray(nSamples, invTimes, "\n");

    // init the necessary objects
    Ox::FunctionsT1CalculatorShmolli<TYPE> functionsObject;
    Ox::FitterAmoebaPrivateNr2<TYPE> fitter;
    Ox::SignCalculatorShmolli<TYPE> signCalculator;
    Ox::StartPointCalculatorShmolli<TYPE> startPointCalculator;
    Ox::CalculatorT1Shmolli<TYPE> calculator;

    // configure
    calculator.setFunctionsT1(&functionsObject);
    calculator.setFitter(&fitter);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(invTimes);
    calculator.setSigPha(sigMag);
    calculator.setSigMag(sigPha);

    calculator.calculate();

    KWUtil::printArray(nSamples, calculator.getSigns(), "\n");
    KWUtil::printArray(nSamples, calculator.getSignal(), "\n");

    std::cout << std::endl ;
    std::cout << originalShmolliDicomReader._imageA->GetPixel(index2d) << " " << calculator.getResults().A << std::endl ;
    std::cout << originalShmolliDicomReader._imageB->GetPixel(index2d) << " " <<calculator.getResults().B << std::endl ;
    std::cout << originalShmolliDicomReader._imageT1star->GetPixel(index2d) << " " <<calculator.getResults().T1star << std::endl ;

//    EXPECT_DOUBLE_EQ(calculator.getResults().A, 0);
//    EXPECT_DOUBLE_EQ(calculator.getResults().B, 0);
//    EXPECT_DOUBLE_EQ(calculator.getResults().T1star, 0);

//    originalShmolliDicomReader._visualise = true;
//    originalShmolliDicomReader.visualise();

}

#endif // USE_PRIVATE_NR2
#endif // USE_ITK