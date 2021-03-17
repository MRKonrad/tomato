/*!
 * \file OxCalculatorT2_factory_test.cpp
 * \author Konrad Werys
 * \date 2019/11/25
 */

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "CmakeConfigForTomato.h"
#ifdef USE_PRIVATE_NR2
#ifdef USE_VNL
#ifdef USE_YAML

#include "TomatoOptions.h"
#include "OxFactoryOfCalculators.h"
#include "OxFactoryOfFitters.h"
#include "OxFactoryOfModels.h"

std::map <std::string, double> CalculateT2UsingFactory(Ox::TomatoOptions<double> &options, double *sigMag) {


    // initialise the necessary objects
    Ox::Calculator <double> *calculator = Ox::FactoryOfCalculators<double>::newByFactory(&options);
    Ox::Model<double> *model = Ox::FactoryOfModels<double>::newByFactory(&options);
    Ox::Fitter <double> *fitter = Ox::FactoryOfFitters<double>::newByFactory(&options);

    // configure
    calculator->setModel(model);
    calculator->setFitter(fitter);

    // set the data
    calculator->setNSamples((int)options.echo_times.size());
    calculator->setEchoTimes(&options.echo_times[0]);
    calculator->setSigMag(sigMag);

    calculator->calculate();

    std::map <std::string, double> results = calculator->getResults();

    delete model;
    delete fitter;
    delete calculator;

    return results;
}

TEST(OxCalculatorT2_factory, blood) {

    typedef double TYPE;

    char filePath [] = "testData/tomatoConfig_T2_inputDirPaths.yaml";
    Ox::TomatoOptions<double> options(filePath);
    TYPE signal[] = {100.0,   36.8,   13.5,    5.0,    1.8,    0.7,    0.3,    0.1,    0.03,    0.01,    0.004};

    std::map <std::string, double> results = CalculateT2UsingFactory(options, signal);


    EXPECT_NEAR(results["A"],  0, 1e-1);
    EXPECT_NEAR(results["B"],  100, 1e-1);
    EXPECT_NEAR(results["T2"], 50, 1e-1);
}

#endif // USE_YAML
#endif // USE_VNL
#endif // USE_PRIVATE_NR2
