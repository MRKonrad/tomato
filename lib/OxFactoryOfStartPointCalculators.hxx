/*!
 * \file OxFactoryOfStartPointCalculators.hxx
 * \author Konrad Werys
 * \date 2019/11/26
 */

#ifndef Tomato_OXFACTORYOFStartPointCalculators_HXX
#define Tomato_OXFACTORYOFStartPointCalculators_HXX

#include "OxStartPointCalculatorBasic.h"
#include "OxStartPointCalculatorShmolli.h"

namespace Ox {

    template < typename TYPE >
    StartPointCalculator<TYPE>*
    FactoryOfStartPointCalculators<TYPE>
    ::newByFactory(TomatoOptions<TYPE> *opts){
        switch (opts->start_point_calc_method){
            case Basic: {
                return new StartPointCalculatorBasic<TYPE>();
            }
            case StartPointSHMOLLI: {
                return new StartPointCalculatorShmolli<TYPE>();
            }
            case NoStartPointCalculators: {
                return 0;
            }
            default:
                throw std::runtime_error("start_point_calc_method object not available");
        }
    }

    template < typename TYPE >
    void
    FactoryOfStartPointCalculators<TYPE>
    ::disp(int start_point_calc_method){

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

} // namespace Ox

#endif //Tomato_OXFACTORYOFStartPointCalculators_HXX
