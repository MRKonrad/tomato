/*!
 * \file OxFactoryOfFunctions.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFFUNCTIONS_H
#define Tomato_OXFACTORYOFFUNCTIONS_H

#include "CmakeConfigForTomato.h"
#include "tomatolib_export.h"

#include "OxModel.h"

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;


    static const char *modelTypeNames[] = {
            "ModelT1ThreeParam",
            "ModelT1TwoParam",
            "ModelT1Shmolli",
            "ModelT2ThreeParam",
            "ModelT2TwoParam"
    };

    enum modelType_t {
        _ModelT1ThreeParam = 0,
        _ModelT1TwoParam = 1,
        _ModelT1Shmolli = 2,
        _ModelT2ThreeParam = 3,
        _ModelT2TwoParam = 4,
        lastFunctorType = _ModelT2TwoParam
    };

    static int modelAvailability[] = {
            1, // ModelT1ThreeParam
            1, // ModelT1TwoParam
            1, // ModelT1Shmolli
            1, // ModelT2ThreeParam
            1  // ModelT2TwoParam
    };

    template<typename TYPE>
    class FactoryOfModels {
    public:

        static Model<TYPE>* newByFactory(TomatoOptions<TYPE> *opts);

        static void disp(int model_type = -1);
    };

} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxFactoryOfModels.hxx"
#endif // TOMATOLIB_COMPILED

#endif //Tomato_OXFACTORYOFFUNCTIONS_H
