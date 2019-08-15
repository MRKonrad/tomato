/*!
 * \file OxFactoryOfCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFCalculators_H
#define Tomato_OXFACTORYOFCalculators_H

#include "CmakeConfigForTomato.h"
#include "OxCalculatorT1Molli.h"
#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1Shmolli.h"
#include "OxCalculatorT1ShmolliOriginal.h"
#endif


namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;

    static const char *calculatorsTypeNames[] = {
            "T1_MOLLI",
            "T1_SHMOLLI",
            "T1_SHMOLLI_original",
    };

    enum calculatorsType_t {
        T1_MOLLI = 0,
        T1_SHMOLLI = 1,
        T1_SHMOLLI_original = 2,
        lastCalculatorType = T1_SHMOLLI_original
    };

    static int calculatorsAvailability[] = {
            1, // T1_MOLLI
#ifdef USE_PRIVATE_NR2
            1, // T1_SHMOLLI
            1 // T1_SHMOLLI_original
#else
            0, // T1_SHMOLLI
            0 // T1_SHMOLLI_original
#endif
    };

    template<typename TYPE>
    class FactoryOfCalculators {
    public:

        static CalculatorT1<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            CalculatorT1<TYPE> *calculatorT1 = 0; //nullpointer
            switch (opts->parameter_to_map){
                case T1_MOLLI: {
                    calculatorT1 = new CalculatorT1Molli<TYPE>();
                    break;
                }
#ifdef USE_PRIVATE_NR2
                case T1_SHMOLLI: {
                    calculatorT1 = new CalculatorT1Shmolli<TYPE>();
                    break;
                }
                case T1_SHMOLLI_original: {
                    calculatorT1 = new CalculatorT1ShmolliOriginal<TYPE>();
                    break;
                }
#endif
                default:
                    throw std::runtime_error("parameter_to_map not available");

            }

            calculatorT1->setMeanCutOff(opts->mean_cut_off);

            return calculatorT1;
        }

        static void disp(int parameter_to_map = -1){

            if (parameter_to_map >= 0) {
                printf("%-30s%-20s", " parameter_to_map: ", calculatorsTypeNames[parameter_to_map]);
            }

            printf("options: [ ");

            for (int i = 0; i < lastCalculatorType+1; i++){
                if(calculatorsAvailability[i]){
                    printf("%s ", calculatorsTypeNames[i]);
                }
            }

            printf("] \n");
        }
    };

} // namespace Ox

#endif //Tomato_OXFACTORYOFCalculators_H
