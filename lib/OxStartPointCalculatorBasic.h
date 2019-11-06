/*!
 * \file OxStartPointCalculatorBasic.h
 * \author Konrad Werys
 * \date 2018/08/10
 */

#ifndef Tomato_OXSTARTPOINTCALCULATORBasic_H
#define Tomato_OXSTARTPOINTCALCULATORBasic_H

#include "OxStartPointCalculator.h"

namespace Ox {

    /**
     * \class StartPointCalculatorBasic
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class StartPointCalculatorBasic : public StartPointCalculator<MeasureType>{

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateStartPoint(){
            for (int i = 0; i < this->getNDims(); i++){
                this->getCalculatedStartPoint()[i] = this->_InputStartPoint[i];
            }
            return 0; // EXIT_SUCCESS
        };

        /**
         * cloning
         * @return
         */
        virtual StartPointCalculator<MeasureType> *newByCloning() { return new StartPointCalculatorBasic<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculatorBasic(){};

    };
} //namespace Ox

#endif //Tomato_OXStartPointCalculatorBasic_H
