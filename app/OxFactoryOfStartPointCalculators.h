/*!
 * \file OxFactoryOfStartPointCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFStartPointCalculators_H
#define Tomato_OXFACTORYOFStartPointCalculators_H

#include "CmakeConfigForTomato.h"

#include "OxStartPointCalculatorDefault2Dims.h"
#include "OxStartPointCalculatorDefault3Dims.h"
#ifdef USE_PRIVATE_NR2
#include "OxStartPointCalculatorShmolli.h"
#endif

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;


    static const char *startPointCalculatorsTypeNames[] = {
            "DefaultThreeParam",
            "DefaultTwoParam",
            "StartPointSHMOLLI"
    };

    enum startPointCalculatorsType_t {
        DefaultThreeParam = 0,
        DefaultTwoParam = 1,
        StartPointSHMOLLI = 2,
        lastStartPointCalculatorType = StartPointSHMOLLI
    };

    static int startPointCalculatorsAvailability[] = {
            1, // DefaultThreeParam
            1, // DefaultTwoParam
#ifdef USE_PRIVATE_NR2
            1, // StartPointSHMOLLI
#else
            0, // StartPointSHMOLLI
#endif
    };


    template<typename TYPE>
    class FactoryOfStartPointCalculators {
    public:

        static StartPointCalculator<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            switch (opts->start_point_calc_method){
                case DefaultThreeParam: {
                    return new StartPointCalculatorDefault3Dims<TYPE>();
                }
                case DefaultTwoParam: {
                    return new StartPointCalculatorDefault2Dims<TYPE>();
                }
#ifdef USE_PRIVATE_NR2
                case StartPointSHMOLLI: {
                    return new StartPointCalculatorShmolli<TYPE>();
                }
#endif
                default:
                    throw std::runtime_error("start_point_calc_method object not available");
            }
        }

        static void disp(int start_point_calc_method = -1){

            if (start_point_calc_method >= 0) {
                printf("%-30s%-20s", " start_point_calc_method: ", startPointCalculatorsTypeNames[start_point_calc_method]);
            }

            printf("options: [ ");

            for (int i = 0; i < lastStartPointCalculatorType+1; i++){

                if(startPointCalculatorsAvailability[i]){
                    printf("%s ", startPointCalculatorsTypeNames[i]);
                }
            }

            printf("] \n");
        }
    };

} // namespace Ox

#endif //Tomato_OXFACTORYOFStartPointCalculators_H
