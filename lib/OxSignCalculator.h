/*!
 * \file OxSignCalculator.h
 * \author Konrad Werys
 * \date 2018/08/29
 */

#ifndef OXSHMOLLI2_OXSIGNCALCULATOR_H
#define OXSHMOLLI2_OXSIGNCALCULATOR_H

namespace Ox {

    /**
     * \class SignCalculator
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class SignCalculator{

    public:
        virtual int calculateSign() = 0;

    protected:
        MeasureType* _InvTimes;
        MeasureType* _SigMag; // original one
        MeasureType* _SigPha; // original one
        MeasureType* _Signal; // we will be working with this one
        MeasureType* _Signs;  // we will be working with this one

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        ~SignCalculator(){};

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXSIGNCALCULATOR_H
