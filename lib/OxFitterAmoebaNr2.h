/*!
 * \file OxFitterAmoebaNr2.h
 * \author Konrad Werys
 * \date 2018/08/06
 */

#ifndef Tomato_OXFITTERAMOEBANr2_H
#define Tomato_OXFITTERAMOEBANr2_H

#include "CmakeConfigForTomato.h"
#ifdef USE_NR2

#include "OxFitter.h"

extern "C"{
#include "nr_modified.h"
#include "nrutil.h"
};


namespace Ox {

    namespace Ugly {
        /*****************/
        /** ugly globals */
        /*****************/
        // if you want to increase it beyond, you have to add more lines
        const int NR_MAX_THREADS = 8;

        // Numerical Recipes Amoeba needs a pointer to a function to be passed with just one argument - minimization
        // parameters. We need some more
        // solution taken from https://isocpp.org/wiki/faq/pointers-to-members
        // it needs declaring globals, so I am using an arrays to have separate global for each thread

        // static because https://stackoverflow.com/questions/14425262/why-include-guards-do-not-prevent-multiple-function-definitions

        // global array of pointers to OxFunctorT1adapterNr3
        static FunctionsT1<float> *globalFunctonsT1Nr2Array[NR_MAX_THREADS]; // could possibly go up to ITK_MAX_THREADS

        // global wrapper functions. Exactly as in https://isocpp.org/wiki/faq/pointers-to-members
        static float f_wrapper0(float *params) { globalFunctonsT1Nr2Array[0]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[0]->calcCostValue(); }
        static float f_wrapper1(float *params) { globalFunctonsT1Nr2Array[1]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[1]->calcCostValue(); }
        static float f_wrapper2(float *params) { globalFunctonsT1Nr2Array[2]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[2]->calcCostValue(); }
        static float f_wrapper3(float *params) { globalFunctonsT1Nr2Array[3]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[3]->calcCostValue(); }
        static float f_wrapper4(float *params) { globalFunctonsT1Nr2Array[4]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[4]->calcCostValue(); }
        static float f_wrapper5(float *params) { globalFunctonsT1Nr2Array[5]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[5]->calcCostValue(); }
        static float f_wrapper6(float *params) { globalFunctonsT1Nr2Array[6]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[6]->calcCostValue(); }
        static float f_wrapper7(float *params) { globalFunctonsT1Nr2Array[7]->copyToParameters(params+1); return globalFunctonsT1Nr2Array[7]->calcCostValue(); }

        // for convenience I put the wrapper functions into an array
        static float (*f_wrapperArray[NR_MAX_THREADS])(float *params)  = {
            f_wrapper0,
            f_wrapper1,
            f_wrapper2,
            f_wrapper3,
            f_wrapper4,
            f_wrapper5,
            f_wrapper6,
            f_wrapper7,
        };
    }

    /**
     * /class FitterAmoebaNr2
     * /brief Not usable.
     * 1. Gives off results
     * 2. If reaches max iterations (which is defined by #define in the .c file, so I cannot change it) it exits the
     * program.
     * @tparam MeasureType
     */
    template<typename MeasureType>
    class FitterAmoebaNr2 : public Fitter<MeasureType> {

    public:

        /**
         * cloning
         * @return
         */
        virtual Fitter<MeasureType> *newByCloning() { return new FitterAmoebaNr2<MeasureType>(*this); }


        int performFitting() {

            // from now on everything depends on the thread we are currently using
            int threadId = this->_ThreadId;
            if (threadId < 0 || threadId >= Ugly::NR_MAX_THREADS){
                throw std::runtime_error("Incorrect threadID");
            }

            // use global ugliness
            Ugly::globalFunctonsT1Nr2Array[threadId] = this->_FunctionsT1;
            float(*func)(float*) = Ugly::f_wrapperArray[threadId];
            int nDims = this->_FunctionsT1->getNDims();

            // store start point
            float *startPoint = new float[nDims];
            KWUtil::copyArrayToArray(nDims, startPoint, this->_FunctionsT1->getParameters());


            // modified NR example code
            int MP = nDims + 1;
            int NP = nDims;
            float FTOL = this->getFTolerance();

            int i, nfunc, j, ndim = NP;
            float *x, *y, **p;

            // alloc
            x = vector(1, NP);
            y = vector(1, MP);
            p = matrix(1, MP, 1, NP);

            // init the simples
            for (i = 1; i <= MP; i++) {
                for (j = 1; j <= NP; j++)
                    if (i != j+1)
                        x[j] = p[i][j] = startPoint[j-1];
                    else
                        x[j] = p[i][j] = startPoint[j-1] * 1.4;
                y[i] = func(x);
            }

            // printing
            if (this->_Verbose) {
                printf("\n%3s %10s %12s %12s %14s\n\n",
                       "i", "x[i]", "y[i]", "z[i]", "function");
                for (i = 1; i <= MP; i++) {
                    printf("%3d ", i);
                    for (j = 1; j <= NP; j++) printf("%12.6f ", p[i][j]);
                    printf("%12.6f\n", y[i]);
                }
            }

            // minimise
            amoeba(p, y, ndim, FTOL, func, &nfunc);

            // copy results
            this->_FunctionsT1->copyToParameters(p[1]+1);

            // printing
            if (this->_Verbose) {
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
            }

            // dealloc
            free_matrix(p, 1, MP, 1, NP);
            free_vector(y, 1, MP);
            free_vector(x, 1, NP);
            delete [] startPoint;

            return 0; // EXIT_SUCCESS
        }
    };

} // namespace Ox

#endif //Tomato_OXFITTERAMOEBANr2_H

#endif // USE_NR2