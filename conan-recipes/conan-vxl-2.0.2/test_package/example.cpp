#include <iostream>
#include <vnl/vnl_math.h>
#include <vnl/algo/vnl_amoeba.h>
#include <vnl/algo/vnl_levenberg_marquardt.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/vnl_least_squares_function.h>

struct my_rosenbrock_cost_functor : public vnl_cost_function {

    my_rosenbrock_cost_functor(int nDims) : vnl_cost_function(nDims){};

    double f(vnl_vector<double> const& params)
    {
        double x = params[0];
        double y = params[1];
        return vnl_math::sqr(10*(y-x*x)) + vnl_math::sqr(1-x);
    };
};

struct my_ls_functor : public vnl_least_squares_function {

    my_ls_functor() : vnl_least_squares_function(2, 2, vnl_least_squares_function::no_gradient){};

    virtual void f(vnl_vector<double> const& params, vnl_vector<double>& fx)
    {
        fx[0] = params[0]*params[0] + params[1]*params[1] - 1; /* unit circle    x^2+y^2=1 */
        fx[1] = params[1] - params[0]*params[0];               /* standard parabola  y=x^2 */
    }
};

int main()
{
    {
        my_rosenbrock_cost_functor f(2);
        vnl_amoeba minimizer(f);
        double temp[] = {10, 20};
        vnl_vector<double> params(temp, 2);
        minimizer.minimize(params);
        std::cout << params << std::endl;
    }
    {
        my_ls_functor f;
        vnl_levenberg_marquardt fitter(f);
        double temp[] = {2, 2};
        vnl_vector<double> params(temp, 2);
        fitter.minimize(params);
        std::cout << params << std::endl;
    }
    return 0;
}
