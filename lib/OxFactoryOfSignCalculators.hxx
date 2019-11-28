/*!
 * \file OxFactoryOfSignCalculators.hxx
 * \author Konrad Werys
 * \date 2019/11/26
 */

#ifndef Tomato_OXFACTORYOFSignCalculators_HXX
#define Tomato_OXFACTORYOFSignCalculators_HXX

#include "CmakeConfigForTomato.h"
#include "OxSignCalculatorNoSign.h"
#include "OxSignCalculatorRealImag.h"
#include "OxSignCalculatorShmolli.h"

namespace Ox {
    template < typename TYPE >
    SignCalculator<TYPE>*
    FactoryOfSignCalculators<TYPE>
    ::newByFactory(TomatoOptions<TYPE> *opts){
        switch (opts->sign_calc_method){
            case NoSign: {
                return new SignCalculatorNoSign<TYPE>();
            }
            case RealImag: {
                return new SignCalculatorRealImag<TYPE>();
            }
            case MagPhase: {
                return new SignCalculatorShmolli<TYPE>();
            }
            case None: {
                return 0;
            }
            default:
                throw std::runtime_error("sign_calc_method not available");
        }
    }

    template < typename TYPE >
    void
    FactoryOfSignCalculators<TYPE>
    ::disp(int sign_calc_method){

        if (sign_calc_method >= 0) {
            printf("%-28s%-22s", " sign_calc_method: ", signCalculatorsTypeNames[sign_calc_method]);
        }

        printf("options: [ ");

        for (int i = 0; i < lastSignCalculatorType+1; i++){

            if(signCalculatorsAvailability[i]){
                printf("%s ", signCalculatorsTypeNames[i]);
            }
        }

        printf("] \n");
    }

} // namespace Ox

#endif //Tomato_OXFACTORYOFSignCalculators_HXX
