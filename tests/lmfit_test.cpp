/*!
 * \file lmfit_test.cpp
 * \author Konrad Werys
 * \date 2019/08/08
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_LMFIT

#include "gtest/gtest.h"
#include "OxTestData.h"

#include "lmmin.h"

void evaluate_nonlin1(
        const double *p, int n, const void *data, double *f, int *info )
{
    f[0] = p[0]*p[0] + p[1]*p[1] - 1; /* unit circle    x^2+y^2=1 */
    f[1] = p[1] - p[0]*p[0];          /* standard parabola  y=x^2 */
}

TEST(lmfit, example_copied_from_lmfit_code) {

    bool verbose = false;

    int n = 2;   /* dimension of the problem */
    double p[2]; /* parameter vector p=(x,y) */

    /* auxiliary parameters */
    lm_control_struct control = lm_control_double;
    lm_status_struct  status;
    if (verbose) {
        control.verbosity = 3;
    }

    p[0] = 2;
    p[1] = 2;

    /* the minimization */
    if (verbose) {
        printf("Minimization:\n");
    }

    lmmin(n, p, n, NULL, NULL, evaluate_nonlin1, &control, &status);

    if (verbose) {
        /* print results */
        printf("\n");
        printf("lmmin status after %d function evaluations:\n  %s\n",
               status.nfev, lm_infmsg[status.outcome]);

        printf("\n");
        printf("Solution:\n");
        printf("  x = %19.11f\n", p[0]);
        printf("  y = %19.11f\n", p[1]);
        printf("  d = %19.11f => ", status.fnorm);

        /* convergence of lmfit is not enough to ensure validity of the solution */
        if (status.fnorm >= control.ftol)
            printf("not a valid solution, try other starting values\n");
        else
            printf("valid, though not the only solution: "
                   "try other starting values\n");
    }
    EXPECT_NEAR(p[0], 0.786, 1e-2);
    EXPECT_NEAR(p[1], 0.618, 1e-2);
}



void evaluate_T1(const double *parameters, int nSamples, const void *data_void, double *residuals, int *info )
{

    double A = parameters[0];
    double B = parameters[1];
    double T1star = parameters[2];

    // casting the pointer data_void
    double *data = (double*)data_void;

    for (unsigned int i = 0; i < nSamples; i++) {
        // decoding the data from the data_void pointer
        double invTime = data[i];
        double measured = data[i+nSamples];
        double calculated = 0;

        calculated = A - B * std::exp( -invTime/ T1star );

        residuals[i] = calculated - measured;
    }
}

#ifdef USE_YAML
TEST(lmfit, simpleT1_test) {

    bool verbose = false;

    typedef double TYPE;

    char filePath [] = "testData/T1_myocardium.yaml";
    Ox::TestData<TYPE> testData(filePath);
    int nSamples = testData.getNSamples();

    // you can pass a single data pointer to lmfit models, here I use a single array
    double data[14];
    KWUtil::copyArrayToArray(nSamples, data, testData.getInvTimesPtr());
    KWUtil::copyArrayToArray(nSamples, &data[7], testData.getSignalPtr());

    int n = 3;   /* dimension of the problem */
    double parameters[3]; /* parameter vector p=(x,y) */

    /* auxiliary parameters */
    lm_control_struct control = lm_control_double;
    lm_status_struct  status;
    if (verbose) {
        control.verbosity = 3;
    }

    // starting point
    parameters[0] = 100;
    parameters[1] = 200;
    parameters[2] = 1000;

    /* the minimization */
    if (verbose){
        printf("Minimization:\n");
    }

    lmmin(n, parameters, nSamples, NULL, data, evaluate_T1, &control, &status);

    if (verbose) {
        /* print results */
        printf("\n");
        printf("lmmin status after %d function evaluations:\n  %s\n",
               status.nfev, lm_infmsg[status.outcome]);

        printf("\n");
        printf("Solution:\n");
        printf("  x = %19.11f\n", parameters[0]);
        printf("  y = %19.11f\n", parameters[1]);
        printf("  y = %19.11f\n", parameters[2]);
        printf("  d = %19.11f => ", status.fnorm);

        /* convergence of lmfit is not enough to ensure validity of the solution */
        if (status.fnorm >= control.ftol)
            printf("not a valid solution, try other starting values\n");
        else
            printf("valid, though not the only solution: "
                   "try other starting values\n");

        std::cout << "control.patience: " << control.patience << std::endl;
    }

    EXPECT_NEAR(parameters[0], testData.getResultsMolli()[0], 1e-2);
    EXPECT_NEAR(parameters[1], testData.getResultsMolli()[1], 1e-2);
    EXPECT_NEAR(parameters[2], testData.getResultsMolli()[2], 1e-2);
}
#endif // USE_YAML

#endif // USE_LMFIT