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


    static const char *modelTypeNames[] = {
            "ModelT1ThreeParam",
            "ModelT1TwoParam",
            "ModelT1Shmolli",
            "ModelT2ThreeParam"
    };

    enum modelType_t {
        _ModelT1ThreeParam = 0,
        _ModelT1TwoParam = 1,
        _ModelT1Shmolli = 2,
        _ModelT2ThreeParam = 3,
        lastFunctorType = _ModelT2ThreeParam
    };

    static int modelAvailability[] = {
            1, // ModelT1ThreeParam
            1, // ModelT1TwoParam
            1, // ModelT1Shmolli
            1  // ModelT2ThreeParam
    };

    template<typename TYPE>
    class FactoryOfModels {
    public:

        static Model<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            switch (opts->model_type){
                case _ModelT1ThreeParam: {
                    return new ModelT1ThreeParam<TYPE>();
                }
                case _ModelT1TwoParam: {
                    return new ModelT1TwoParam<TYPE>();
                }
                case _ModelT1Shmolli: {
                    return new ModelT1Shmolli<TYPE>();
                }
                case _ModelT2ThreeParam: {
                    return new ModelT2ThreeParam<TYPE>();
                }
                default:
                    throw std::runtime_error("model_type not available");
            }
        }

        static void disp(int model_type = -1){

            if (model_type >= 0) {
                printf("%-28s%-22s", " model_type: ", modelTypeNames[model_type]);
            }

            printf("options: [ ");

            for (int i = 0; i < lastFunctorType+1; i++){

                if(modelAvailability[i]){
                    printf("%s ", modelTypeNames[i]);
                }
            }

            printf("] \n");
        }
    };

} // namespace Ox

#endif //Tomato_OXFACTORYOFFUNCTIONS_H
