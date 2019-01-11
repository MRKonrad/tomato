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

#ifdef USE_PRIVATE_NR2
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
#else
    static const char *calculatorsTypeNames[] = {
            "T1_MOLLI",
    };

    enum calculatorsType_t {
        T1_MOLLI = 0,
        lastCalculatorType = T1_MOLLI
    };
#endif

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
					calculatorT1.phaseRange = 2048.*2;
                    break;
                }
#endif
            }

            calculatorT1->setMeanCutOff(opts->mean_cut_off);

            return calculatorT1;
        }
    };

} // namespace Ox

#endif //Tomato_OXFACTORYOFCalculators_H
