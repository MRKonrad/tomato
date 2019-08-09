///*!
// * \file lmfit_test.cpp
// * \author Konrad Werys
// * \date 2019/08/08
// */
//
//#include "gtest/gtest.h"
//#include "lmmin.h"
//
//void evaluate_nonlin1(
//        const double *p, int n, const void *data, double *f, int *info )
//{
//    f[0] = p[0]*p[0] + p[1]*p[1] - 1; /* unit circle    x^2+y^2=1 */
//    f[1] = p[1] - p[0]*p[0];          /* standard parabola  y=x^2 */
//}
//
//TEST(lmfit, first_test) {
//
//    std::cout << "aaa" << std::endl;
//    int n = 2;   /* dimension of the problem */
//    double p[2]; /* parameter vector p=(x,y) */
//
//    /* auxiliary parameters */
//    lm_control_struct control = lm_control_double;
//    lm_status_struct  status;
//    control.verbosity  = 31;
//
//    p[0] = 2;
//    p[1] = 2;
//
//    /* the minimization */
//    printf("Minimization:\n");
//    lmmin(n, p, n, NULL, NULL, evaluate_nonlin1, &control, &status);
//
//    /* print results */
//    printf("\n");
//    printf("lmmin status after %d function evaluations:\n  %s\n",
//           status.nfev, lm_infmsg[status.outcome]);
//
//    printf("\n");
//    printf("Solution:\n");
//    printf("  x = %19.11f\n", p[0]);
//    printf("  y = %19.11f\n", p[1]);
//    printf("  d = %19.11f => ", status.fnorm);
//
//    /* convergence of lmfit is not enough to ensure validity of the solution */
//    if( status.fnorm >= control.ftol )
//        printf("not a valid solution, try other starting values\n");
//    else
//        printf("valid, though not the only solution: "
//               "try other starting values\n");
//}
