/*!
 * \file OxFactoryOfFunctions.hxx
 * \author Konrad Werys
 * \date 2019/11/26
 */

#ifndef Tomato_OXFACTORYOFFUNCTIONS_HXX
#define Tomato_OXFACTORYOFFUNCTIONS_HXX

#include "OxModelT1ThreeParam.h"
#include "OxModelT1TwoParam.h"
#include "OxModelT1Shmolli.h"
#ifdef USE_PRIVATE_NR2
#include "OxModelT2ThreeParam.h"
#include "OxModelT2TwoParam.h"
#include "OxModelT2OneParam.h"
#endif

namespace Ox {
    template < typename TYPE >
    Model<TYPE>*
    FactoryOfModels<TYPE>
    ::newByFactory(TomatoOptions<TYPE> *opts){
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
#ifdef USE_PRIVATE_NR2
            case _ModelT2ThreeParam: {
                return new ModelT2ThreeParam<TYPE>();
            }
            case _ModelT2TwoParam: {
                return new ModelT2TwoParam<TYPE>();
            }
            case _ModelT2OneParam: {
                return new ModelT2OneParam<TYPE>();
            }
#endif
            default:
                throw std::runtime_error("model_type not available");
        }
    }

    template < typename TYPE >
    void
    FactoryOfModels<TYPE>
    ::disp(int model_type){

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

} // namespace Ox

#endif //Tomato_OXFACTORYOFFUNCTIONS_HXX
