/*!
 * \file OxFitterAmoebaNr2.h
 * \author Konrad Werys
 * \date 2018/08/06
 */

#ifndef OXSHMOLLI2_OXFITTERAMOEBANr2_H
#define OXSHMOLLI2_OXFITTERAMOEBANr2_H

#include "CmakeConfigForOxShmolli2.h"
#ifdef USE_NR2

#include "OxFitter.h"

extern "C"{
#include "nr_modified.h"
#include "nrutil.h"
};

float func(float x[])
{
    return 0.6-bessj0(SQR(x[1]-0.5)+SQR(x[2]-0.6)+SQR(x[3]-0.7));
}

namespace Ox {
    template<typename MeasureType>
    class FitterAmoebaNr2 : public Fitter<MeasureType> {

    public:

        FitterAmoebaNr2() {
            _nDims = 3;
        }

        int performFitting() {
            int MP = _nDims + 1;
            int NP = _nDims;
            float FTOL = this->getFTolerance();

            int i, nfunc, j, ndim = NP;
            float *x, *y, **p;

            x = vector(1, NP);
            y = vector(1, MP);
            p = matrix(1, MP, 1, NP);
            for (i = 1; i <= MP; i++) {
                for (j = 1; j <= NP; j++)
                    x[j] = p[i][j] = (i == (j + 1) ? 1.0 : 0.0);
                y[i] = func(x);
            }
            amoeba(p, y, ndim, FTOL, func, &nfunc);
            printf("\nNumber of function evaluations: %3d\n", nfunc);
            printf("Vertices of final 3-d simplex and\n");
            printf("function values at the vertices:\n\n");
            printf("%3s %10s %12s %12s %14s\n\n",
                   "i", "x[i]", "y[i]", "z[i]", "function");
            for (i = 1; i <= MP; i++) {
                printf("%3d ", i);
                for (j = 1; j <= NP; j++) printf("%12.6f ", p[i][j]);
                printf("%12.6f\n", y[i]);
            }
            printf("\nTrue minimum is at (0.5,0.6,0.7)\n");
            free_matrix(p, 1, MP, 1, NP);
            free_vector(y, 1, MP);
            free_vector(x, 1, NP);

            return 0; // EXIT_SUCCESS
        }

    protected:
        int _nDims;
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFITTERAMOEBANr2_H

#endif // USE_NR2