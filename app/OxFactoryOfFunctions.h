/*!
 * \file OxFactoryOfFunctions.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFFUNCTIONS_H
#define Tomato_OXFACTORYOFFUNCTIONS_H

#include "CmakeConfigForTomato.h"

#include "OxModelT1ThreeParam.h"
#include "OxModelT1TwoParam.h"
#include "OxModelT1Shmolli.h"
#include "OxModelT2ThreeParam.h"

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;


    static const char *functionsTypeNames[] = {
            "FunctionsThreeParam",
            "FunctionsTwoParam",
            "FunctionsShmolli",
            "ModelT2ThreeParam"
    };

    enum functionsType_t {
        FunctionsThreeParam = 0,
        FunctionsTwoParam = 1,
        FunctionsShmolli = 2,
        _ModelT2ThreeParam = 3,
        lastFunctorType = _ModelT2ThreeParam
    };

    static int functionsAvailability[] = {
            1, //FunctionsThreeParam
            1, //FunctionsTwoParam
            1, // FunctionsShmolli
            1  // ModelT2ThreeParam
    };

    template<typename TYPE>
    class FactoryOfFunctions {
    public:

        static Model<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            switch (opts->functions_type){
                case FunctionsThreeParam: {
                    return new ModelT1ThreeParam<TYPE>();
                }
                case FunctionsTwoParam: {
                    return new ModelT1TwoParam<TYPE>();
                }
                case FunctionsShmolli: {
                    return new ModelT1Shmolli<TYPE>();
                }
                case _ModelT2ThreeParam: {
                    return new ModelT2ThreeParam<TYPE>();
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
