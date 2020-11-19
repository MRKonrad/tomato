#include <iostream>
#include <vnl/vnl_math.h>
#include <vnl/algo/vnl_amoeba.h>
#include <vnl/vnl_cost_function.h>

struct my_rosenbrock_functor : public vnl_cost_function {

    my_rosenbrock_functor(int nDims) : vnl_cost_function(nDims){};

    double f(vnl_vector<double> const& params)
    {
        double x = params[0];
        double y = params[1];
        return vnl_math::sqr(10*(y-x*x)) + vnl_math::sqr(1-x);
    };
};

int main()
{
    my_rosenbrock_functor f(2);
    vnl_amoeba minimizer(f);
    double temp[] = {10, 20};
    vnl_vector<double> params(temp, 2);
    minimizer.minimize(params);
    std::cout << params << std::endl;
    return 0;
}