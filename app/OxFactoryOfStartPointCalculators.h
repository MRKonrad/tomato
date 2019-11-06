/*!
 * \file OxFactoryOfStartPointCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFStartPointCalculators_H
#define Tomato_OXFACTORYOFStartPointCalculators_H

#include "CmakeConfigForTomato.h"

#include "OxStartPointCalculatorBasic.h"
#include "OxStartPointCalculatorShmolli.h"

namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;


    static const char *startPointCalculatorsTypeNames[] = {
            "Basic",
            "StartPointSHMOLLI"
    };

    enum startPointCalculatorsType_t {
        Basic = 0,
        StartPointSHMOLLI = 1,
        lastStartPointCalculatorType = StartPointSHMOLLI
    };

    static int startPointCalculatorsAvailability[] = {
            1, // Basic
            1, // StartPointSHMOLLI
    };

    template<typename TYPE>
    class FactoryOfStartPointCalculators {
    public:

        static StartPointCalculator<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            switch (opts->start_point_calc_method){
                case Basic: {
                    return new StartPointCalculatorBasic<TYPE>();
                }
                case StartPointSHMOLLI: {
                    return new StartPointCalculatorShmolli<TYPE>();
                }
                default:
                    throw std::runtime_error("start_point_calc_method object not available");
            }
        }

        static void disp(int start_point_calc_method = -1){

            if (start_point_calc_method >= 0) {
                printf("%-28s%-22s", " start_point_calc_method: ", startPointCalculatorsTypeNames[start_point_calc_method]);
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
