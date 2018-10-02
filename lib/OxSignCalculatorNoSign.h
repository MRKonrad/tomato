/*!
 * \file OxSignCalculatorNoSign.h
 * \author Konrad Werys
 * \date 2018/08/13
 */

#ifndef Tomato_OXSignCalculatorNoSign_H
#define Tomato_OXSignCalculatorNoSign_H

#include "OxSignCalculator.h"

namespace Ox {

    /**
     * \class SignCalculatorNoSign
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class SignCalculatorNoSign : public SignCalculator<MeasureType> {

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateSign(){

            for (int i = 0; i < this->getNSamples(); ++i) {
                this->getSignal()[i] = this->getSigMag()[i];
                this->getSigns() [i] = 0;
            }

            return 0; // EXIT_SUCCESS
        }

        /**
         * \brief constructor
         */
        SignCalculatorNoSign(){
            this->setAllPointersToNull();
            this->_nSamples = 0;
        };

        /**
         * \brief copy constructor
         */
        SignCalculatorNoSign(const SignCalculatorNoSign &old){
            this->setAllPointersToNull();
            this->_nSamples = old._nSamples;
        };

        /**
         * cloning
         * @return
         */
        virtual SignCalculator<MeasureType> *newByCloning() { return new SignCalculatorNoSign<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~SignCalculatorNoSign(){};

    };
} //namespace Ox

#endif //Tomato_OXSignCalculatorNoSign_H
