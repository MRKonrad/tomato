/*!
 * \file OxFactoryOfCalculators.hxx
 * \author Konrad Werys
 * \date 2019/11/26
 */

#ifndef Tomato_OXFACTORYOFCalculators_HXX
#define Tomato_OXFACTORYOFCalculators_HXX

#include "OxCalculatorT1WithSignCheck.h"
#include "OxCalculatorT1Shmolli.h"
#ifdef USE_PRIVATE_NR2
#include "OxCalculatorT1ShmolliOriginal.h"
#include "OxCalculatorT2.h"
#include "OxCalculatorT2Truncation.h"
#include "OxCalculatorT2TruncationNoise.h"
#include "OxCalculatorT2Linear.h"
#endif
#include "TomatoOptions.h"
#include "OxFactoryOfCalculators.h"


namespace Ox {

    template < typename TYPE >
    Calculator<TYPE>*
    FactoryOfCalculators<TYPE>
    ::newByFactory(TomatoOptions<TYPE> *opts){
        Calculator<TYPE> *calculator = 0; //nullpointer
        switch (opts->parameter_to_map){
            case T1_MOLLI: {
                calculator = new CalculatorT1WithSignCheck<TYPE>();
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
            case T2_basic: {
                calculator = new CalculatorT2<TYPE>();
                break;
            }
            case T2_truncation: {
                calculator = new CalculatorT2Truncation<TYPE>();
                break;
            }
            case T2_truncation_noise: {
                calculator = new CalculatorT2TruncationNoise<TYPE>();
                break;
            }
            case T2_linear: {
                calculator = new CalculatorT2Linear<TYPE>();
                break;
            }
#endif
            default:
                throw std::runtime_error("parameter_to_map not available");

        }

        calculator->setMeanCutOff(opts->mean_cut_off);

        return calculator;
    }

    template < typename TYPE >
    void
    FactoryOfCalculators<TYPE>
    ::disp(int parameter_to_map){

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

} // namespace Ox

#endif //Tomato_OXFACTORYOFCalculators_HXX
