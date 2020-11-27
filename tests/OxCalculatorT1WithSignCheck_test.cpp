/*!
 * \file OxCalculatorT1WithSignCheck_test.cpp
 * \author Konrad Werys
 * \date 2018/08/01
 */


#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"

#include "OxModelT1ThreeParam.h"
#include "OxFitterAmoebaVnl.h"
#include "OxFitterLevenbergMarquardtVnl.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#include "OxStartPointCalculatorBasic.h"
#include "OxCalculatorT1WithSignCheck.h"

#ifdef USE_ITK
#include "itkTimeProbe.h"
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1WithSignCheck, calculate_doNotCalculateIfMaxIterZero) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    fitterAmoebaVnl.setMaxFunctionEvals(0); // I set the maxFunctionEvals to zero
    calculator.setModel(&model);
    calculator.setFitter(&fitterAmoebaVnl);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(testData.getInvTimesPtr());
    calculator.setSigPha(testData.getSignalPhaPtr());
    calculator.setSigMag(testData.getSignalMagPtr());

    calculator.calculate();

    EXPECT_DOUBLE_EQ(calculator.getResults()["A"], 0);
    EXPECT_DOUBLE_EQ(calculator.getResults()["B"], 0);
    EXPECT_DOUBLE_EQ(calculator.getResults()["T1star"], 0);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1WithSignCheck, calculate_throwIfInvTimesNotSorted) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    TYPE *invTimesNotSorted = new TYPE[nSamples];
    invTimesNotSorted[0] = testData.getSignalPhaPtr()[1];
    invTimesNotSorted[1] = testData.getSignalPhaPtr()[0];
    invTimesNotSorted[2] = testData.getSignalPhaPtr()[2];
    invTimesNotSorted[3] = testData.getSignalPhaPtr()[3];
    invTimesNotSorted[4] = testData.getSignalPhaPtr()[4];
    invTimesNotSorted[5] = testData.getSignalPhaPtr()[5];
    invTimesNotSorted[6] = testData.getSignalPhaPtr()[6];

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculator;

    // configure
    calculator.setModel(&model);
    calculator.setFitter(&fitterAmoebaVnl);
    calculator.setSignCalculator(&signCalculator);
    calculator.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculator.setNSamples(nSamples);
    calculator.setInvTimes(invTimesNotSorted);
    calculator.setSigPha(testData.getSignalMagPtr());
    calculator.setSigMag(testData.getSignalPhaPtr());

    EXPECT_THROW(calculator.calculate(), std::runtime_error);

    delete [] invTimesNotSorted;
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1WithSignCheck, calculate_WithoutSigns) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1;

    // configure
    calculatorT1.setModel(&model);
    calculatorT1.setFitter(&fitterAmoebaVnl);
    calculatorT1.setSignCalculator(&signCalculator);
    calculatorT1.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1.setNSamples(nSamples);
    calculatorT1.setInvTimes(testData.getInvTimesPtr());
    calculatorT1.setSigPha(testData.getSignalPhaPtr());
    calculatorT1.setSigMag(testData.getSignalMagPtr());

    calculatorT1.calculate();

    EXPECT_NEAR(calculatorT1.getResults()["A"], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["B"], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["T1star"], testData.getResultsMolli()[2], 1e-2);
    EXPECT_EQ(calculatorT1.getResults()["timeFlip"], 2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1WithSignCheck, calculate_WithSigns) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1;

    // configure
    calculatorT1.setModel(&model);
    calculatorT1.setFitter(&fitterAmoebaVnl);
    calculatorT1.setSignCalculator(&signCalculator);
    calculatorT1.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1.setNSamples(nSamples);
    calculatorT1.setInvTimes(testData.getInvTimesPtr());
    calculatorT1.setSigPha(testData.getSignalPhaPtr());
    calculatorT1.setSigMag(testData.getSignalMagPtr());

    calculatorT1.calculate();

    EXPECT_NEAR(calculatorT1.getResults()["A"], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["B"], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["T1star"], testData.getResultsMolli()[2], 1e-2);
}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1WithSignCheck, copyConstructor) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterAmoebaVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorRealImag<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1;

    // configure
    calculatorT1.setModel(&model);
    calculatorT1.setFitter(&fitterAmoebaVnl);
    calculatorT1.setSignCalculator(&signCalculator);
    calculatorT1.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1.setNSamples(nSamples);
    calculatorT1.setInvTimes(testData.getInvTimesPtr());
    calculatorT1.setSigPha(testData.getSignalPhaPtr());
    calculatorT1.setSigMag(testData.getSignalMagPtr());

    calculatorT1.setMeanCutOff(123);

    Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1Copy = calculatorT1;

    EXPECT_EQ( calculatorT1.getMeanCutOff(), calculatorT1Copy.getMeanCutOff());
    EXPECT_EQ( calculatorT1.getNSamples(), calculatorT1Copy.getNSamples());
    EXPECT_EQ( calculatorT1.getNDims(), calculatorT1Copy.getNDims());

    // empty object pointers
    EXPECT_THROW(calculatorT1Copy.getModel(), std::runtime_error);
    EXPECT_THROW(calculatorT1Copy.getFitter(), std::runtime_error);

    // empty array pointers
    EXPECT_THROW(calculatorT1Copy.getInvTimes(), std::runtime_error);
    EXPECT_FALSE(calculatorT1Copy.getRepTimes());
    EXPECT_FALSE(calculatorT1Copy.getRelAcqTimes());
    EXPECT_THROW(calculatorT1Copy.getSigMag(), std::runtime_error);
    EXPECT_FALSE(calculatorT1Copy.getSigPha());

    // non-empty pointers of internal arrays
    EXPECT_TRUE(calculatorT1Copy.getSignal());
    EXPECT_TRUE(calculatorT1Copy.getSigns());
    EXPECT_TRUE(calculatorT1Copy.getStartPoint());

}
#endif

#ifdef USE_VNL
TEST(OxCalculatorT1WithSignCheck, calculateFitError) {

    typedef double TYPE;

    char filePath [] = "testData/T1_blood.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1;

    // configure
    calculatorT1.setModel(&model);
    calculatorT1.setFitter(&fitterAmoebaVnl);
    calculatorT1.setSignCalculator(&signCalculator);
    calculatorT1.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1.setNSamples(nSamples);
    calculatorT1.setInvTimes(testData.getInvTimesPtr());
    calculatorT1.setSigPha(testData.getSignalPhaPtr());
    calculatorT1.setSigMag(testData.getSignalMagPtr());

    calculatorT1.calculate();

    EXPECT_NEAR(calculatorT1.getResults()["deltaT1"], 675.17, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["deltaT1star"], 298.23, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["deltaA"], 5.87, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["deltaB"], 6.21, 1e-2);
}
#endif


#ifdef USE_VNL
TEST(OxCalculatorT1WithSignCheck, calculateFitError2) {

    typedef double TYPE;

    double signal[] = {76.7793, 66.6405, 15.8322, 4.97358, 38.978, 62.5192, 76.5024};
    double times[] = {100, 180, 260, 1030, 1942, 2885, 3820 };
    int nSamples = 7;

    // init the necessary objects
    Ox::ModelT1ThreeParam<TYPE> model;
    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterAmoebaVnl;
    Ox::SignCalculatorNoSign<TYPE> signCalculator;
    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
    Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1;

    // configure
    calculatorT1.setModel(&model);
    calculatorT1.setFitter(&fitterAmoebaVnl);
    calculatorT1.setSignCalculator(&signCalculator);
    calculatorT1.setStartPointCalculator(&startPointCalculator);

    // set the data
    calculatorT1.setNSamples(nSamples);
    calculatorT1.setInvTimes(times);
    calculatorT1.setSigMag(signal);

    calculatorT1.calculate();

    EXPECT_NEAR(calculatorT1.getResults()["T1"], 1234.9, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["deltaT1"], 2017.18, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["deltaT1star"], 754.25, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["deltaA"], 28.79, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["deltaB"], 25.66, 1e-2);
    EXPECT_NEAR(calculatorT1.getResults()["R2"], 0.938, 1e-3);
    EXPECT_NEAR(calculatorT1.getResults()["R2Abs"], 0.765, 1e-3);
}
#endif

////TODO: no difference between 5 samples and 7 samples here.
//#ifdef USE_ITK
//
//TEST(OxCalculatorT1WithSignCheck, samples5) {
//
//    typedef double TYPE;
//    int nRepetitions = 10000;
//
//    char filePath5 [] = "testData/T1_blood_5samples.yaml";
//    Ox::TestData<TYPE> testData5(filePath5);
//    int nSamples5 = testData5.getNSamples();
//
//    char filePath7 [] = "testData/T1_blood.yaml";
//    Ox::TestData<TYPE> testData7(filePath7);
//    int nSamples7 = testData7.getNSamples();
//
//    // init the necessary objects
//    Ox::ModelT1ThreeParam<TYPE> model;
//    Ox::FitterLevenbergMarquardtVnl<TYPE> fitterAmoebaVnl;
//    Ox::SignCalculatorRealImag<TYPE> signCalculator;
//    Ox::StartPointCalculatorBasic<TYPE> startPointCalculator;
//    Ox::CalculatorT1WithSignCheck<TYPE> calculatorT1;
//
//    // configure
//    calculatorT1.setModel(&model);
//    calculatorT1.setFitter(&fitterAmoebaVnl);
//    calculatorT1.setSignCalculator(&signCalculator);
//    calculatorT1.setStartPointCalculator(&startPointCalculator);
//
//    // set the data
//    calculatorT1.setNSamples(nSamples5);
//    calculatorT1.setInvTimes(testData5.getInvTimesPtr());
//    calculatorT1.setSigPha(testData5.getSignalPhaPtr());
//    calculatorT1.setSigMag(testData5.getSignalMagPtr());
//
//    printf("\n");
//    itk::TimeProbe clock;
//    clock.Start();
//    for (int i = 0; i < nRepetitions; i++)
//        calculatorT1.calculate();
//    clock.Stop();
//    printf("Calculation5 time: %.12fs.\n", clock.GetTotal());
//
//    // set the data
//    calculatorT1.setNSamples(nSamples7);
//    calculatorT1.setInvTimes(testData7.getInvTimesPtr());
//    calculatorT1.setSigPha(testData7.getSignalPhaPtr());
//    calculatorT1.setSigMag(testData7.getSignalMagPtr());
//
//    clock.Reset();
//    clock.Start();
//    for (int i = 0; i < nRepetitions; i++)
//        calculatorT1.calculate();
//    clock.Stop();
//    printf("Calculation7 time: %.12fs.\n", clock.GetTotal());
//
//}
//
//#endif
