/*!
 * \file lmfit_test.cpp
 * \author Konrad Werys
 * \date 2019/08/08
 */

#include "CmakeConfigForTomato.h"
#ifdef USE_LMFIT

#include "gtest/gtest.h"
#include "lmmin.h"
#include <cmath>
void evaluate_nonlin1(
        const double *p, int n, const void *data, double *f, int *info )
{
    f[0] = p[0]*p[0] + p[1]*p[1] - 1; /* unit circle    x^2+y^2=1 */
    f[1] = p[1] - p[0]*p[0];          /* standard parabola  y=x^2 */
}

TEST(lmfit, first_test) {

    int n = 2;   /* dimension of the problem */
    double p[2]; /* parameter vector p=(x,y) */

    /* auxiliary parameters */
    lm_control_struct control = lm_control_double;
    lm_status_struct  status;
    control.verbosity  = 3;

    p[0] = 2;
    p[1] = 2;

    /* the minimization */
    printf("Minimization:\n");
    lmmin(n, p, n, NULL, NULL, evaluate_nonlin1, &control, &status);

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
    if( status.fnorm >= control.ftol )
        printf("not a valid solution, try other starting values\n");
    else
        printf("valid, though not the only solution: "
               "try other starting values\n");
}



void evaluate_T1(
        const double *parameters, int nSamples, const void *data_void, double *residuals, int *info )
{

    double A = parameters[0];
    double B = parameters[1];
    double T1star = parameters[2];

//    std::cout << parameters[0] << std::endl;
//    std::cout << parameters[1] << std::endl;
//    std::cout << parameters[2] << std::endl;

    double *data = (double*)data_void;
//    std::cout << data[0] << std::endl;
//    std::cout << data[1] << std::endl;
//    std::cout << data[2] << std::endl;

    for (unsigned int i = 0; i < nSamples; i++) {
        double invTime = data[i];
        double measured = data[i+nSamples];
        double calculated = 0;

        calculated = A - B * exp( -invTime/ T1star );

        residuals[i] = calculated - measured;
    }
}

TEST(lmfit, second_test) {

    int n = 3;   /* dimension of the problem */
    double parameters[3]; /* parameter vector p=(x,y) */

    /* auxiliary parameters */
    lm_control_struct control = lm_control_double;
    lm_status_struct  status;
    control.verbosity  = 3;
//    control.patience = 100;

    std::cout << "control.patience: " << control.patience << std::endl;

    parameters[0] = 100;
    parameters[1] = 200;
    parameters[2] = 1000;

    double* data = new double[14];
    data[0] = 100;
    data[1] = 180;
    data[2] = 260;
    data[3] = 1715;
    data[4] = 3337;
    data[5] = 4907;
    data[6] = 6455;
    data[7] = -49;
    data[8] = -40;
    data[9] = -26;
    data[10] = 42;
    data[11] = 59;
    data[12] = 62;
    data[13] = 63;

    /* the minimization */
    printf("Minimization:\n");
    lmmin(n, parameters, 7, NULL, data, evaluate_T1, &control, &status);

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
    if( status.fnorm >= control.ftol )
        printf("not a valid solution, try other starting values\n");
    else
        printf("valid, though not the only solution: "
               "try other starting values\n");

    std::cout << "control.patience: " << control.patience << std::endl;
}

#endif // USE_LMFIT