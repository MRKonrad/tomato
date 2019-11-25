/*!
 * \file OxFactoryOfCalculators.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_OXFACTORYOFCalculators_H
#define Tomato_OXFACTORYOFCalculators_H

#include "CmakeConfigForTomato.h"
#include "OxCalculatorT1Molli.h"
#include "OxCalculatorT1Shmolli.h"
#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1ShmolliOriginal.h"
#endif
#include "OxCalculatorT2.h"
#include "OxCalculatorT2Truncation.h"
#include "TomatoOptions.h"


namespace Ox {

    template<typename TYPE>
    struct TomatoOptions;

    enum paramType_t {
        T1 = 0,
        T2 = 1,
        T2star = 2,
        Perf = 3
    };

    static const char *calculatorsTypeNames[] = {
            "T1_MOLLI",
            "T1_SHMOLLI",
            "T1_SHMOLLI_original",
            "T2",
            "T2_truncation"
    };

    enum calculatorsType_t {
        T1_MOLLI = 0,
        T1_SHMOLLI = 1,
        T1_SHMOLLI_original = 2,
        T2_basic = 3,
        T2_truncation = 4,
        lastCalculatorType = T2_truncation
    };

    static int calculatorsAvailability[] = {
            1, // T1_MOLLI
            1, // T1_SHMOLLI
#ifdef USE_PRIVATE_NR2
            1, // T1_SHMOLLI_original
#else
            0, // T1_SHMOLLI_original
#endif
            1, // T2_basic
            1 // T2_truncation
    };

    static int calculatorsParamsToCalculate[] = {
            T1, // T1_MOLLI
            T1, // T1_SHMOLLI
            T1, // T1_SHMOLLI_original
            T2, // T2_basic
            T2  // T2_truncation
    };

    template<typename TYPE>
    class FactoryOfCalculators {
    public:

        static Calculator<TYPE>* newByFactory(TomatoOptions<TYPE> *opts){
            Calculator<TYPE> *calculator = 0; //nullpointer
            switch (opts->parameter_to_map){
                case T1_MOLLI: {
                    calculator = new CalculatorT1Molli<TYPE>();
                    break;
                }
                case T1_SHMOLLI: {
                    calculator = new CalculatorT1Shmolli<TYPE>();
                    break;
                }
#ifdef USE_PRIVATE_NR2
                case T1_SHMOLLI_original: {
                    calculator = new CalculatorT1ShmolliOriginal<TYPE>();
                    break;
                }
#endif
                case T2_basic: {
//                    CalculatorT2<TYPE> *temp = new CalculatorT2<TYPE>();
//                    temp->SetNoise(&(opts->noise)[0]);
//                    calculator = temp;
                    calculator = new CalculatorT2<TYPE>();
                    break;
                }
                case T2_truncation: {
                    calculator = new CalculatorT2Truncation<TYPE>();
                    break;
                }
                default:
                    throw std::runtime_error("parameter_to_map not available");

            }

            calculator->setMeanCutOff(opts->mean_cut_off);

            return calculator;
        }

        static void disp(int parameter_to_map = -1){

            if (parameter_to_map >= 0) {
                printf("%-28s%-22s", " parameter_to_map: ", calculatorsTypeNames[parameter_to_map]);
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
