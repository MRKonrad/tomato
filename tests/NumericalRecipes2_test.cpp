/*!
 * \file NumericalRecipes_test.cpp
 * \author Konrad Werys
 * \date 2018/08/03
 */

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_NR2

#include "gtest/gtest.h"
#include "OxTestData.h"

extern "C"{
#include "nr_modified.h"
#include "nrutil.h"
};


float myfunk(float array[]){
    return 0;
}

TEST(NumericalRecipes, doAnything) {
    float **p;
    float y[3];
    int ndim = 3;
    float ftol = 1e-12;
    int nfunk = 0;
    amoeba(p, y, ndim, ftol, &myfunk, &nfunk);
}

#endif

