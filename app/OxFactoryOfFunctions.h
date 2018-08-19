/*!
 * \file OxFactoryOfFunctions.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef OXSHMOLLI2_OXFACTORYOFFUNCTIONS_H
#define OXSHMOLLI2_OXFACTORYOFFUNCTIONS_H

#include "OxFunctionsT1Basic.h"

namespace Ox {

    template<typename TYPE>
    struct OxShmolli2Options;

    static const char *functionsTypeNames[] = {
            "FunctionsBasic",
    };

    enum functionsType_t {
        FunctionsBasic = 0,
        lastFunctorType = FunctionsBasic
    };

    template<typename TYPE>
    class FactoryOfFunctions {
    public:

        static FunctionsT1<TYPE>* generateFunctionsObject(OxShmolli2Options<TYPE> *opts){
            switch (opts->functions_type){
                case FunctionsBasic: {
                    return new FunctionsT1Basic<TYPE>();
                }
                default:
                    return 0;
            }
        }
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXFACTORYOFFUNCTIONS_H
