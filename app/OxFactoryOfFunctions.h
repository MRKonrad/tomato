/*!
 * \file OxFactoryOfFunctions.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFFUNCTIONS_H
#define Tomato_OXFACTORYOFFUNCTIONS_H

#include "CmakeConfigForTomato.h"

#include "OxFunctionsT1ThreeParam.h"
#include "OxFunctionsT1TwoParam.h"
#ifdef USE_PRIVATE_NR2
#include "OxFunctionsT1Shmolli.h"
#endif

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;


    static const char *functionsTypeNames[] = {
            "FunctionsThreeParams",
            "FunctionsTwoParams",
            "FunctionsShmolli",
    };

    enum functionsType_t {
        FunctionsThreeParams = 0,
        FunctionsTwoParams = 1,
        FunctionsShmolli = 2,
        lastFunctorType = FunctionsShmolli
    };

    static int functionsAvailability[] = {
            1, //FunctionsThreeParams
            1, //FunctionsTwoParams
            1 // FunctionsShmolli
    };


    template<typename TYPE>
    class FactoryOfFunctions {
    public:

        static FunctionsT1<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            switch (opts->functions_type){
                case FunctionsThreeParams: {
                    return new FunctionsT1ThreeParam<TYPE>();
                }
                case FunctionsTwoParams: {
                    return new FunctionsT1TwoParam<TYPE>();
                }
                case FunctionsShmolli: {
                    return new FunctionsT1Shmolli<TYPE>();
                }
                default:
                    throw std::runtime_error("functions_type not available");
            }
        }

        static void disp(int functions_type = -1){

            if (functions_type >= 0) {
                printf("%-28s%-22s", " functions_type: ", functionsTypeNames[functions_type]);
            }

            printf("options: [ ");

            for (int i = 0; i < lastFunctorType+1; i++){

                if(functionsAvailability[i]){
                    printf("%s ", functionsTypeNames[i]);
                }
            }

            printf("] \n");
        }
    };

} // namespace Ox

#endif //Tomato_OXFACTORYOFFUNCTIONS_H
