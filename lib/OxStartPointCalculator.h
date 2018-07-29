/*!
 * \file OxSignCalculator.h
 * \author Konrad Werys
 * \date 2018/08/29
 */

#ifndef OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H
#define OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H

namespace Ox {

    /**
     * \class StartPointCalculator
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class StartPointCalculator{

    public:
        int calculateStartPoint(){

            return EXIT_SUCCESS;
        };

    protected:
        MeasureType* _Parameters;
        MeasureType* _InvTimes;
        MeasureType* _Signal;
        MeasureType* _Signs;

        MeasureType* _DefaultStartPoint;
        MeasureType* _CalculatedStartPoint;

        StartPointCalculator(){
            _DefaultStartPoint = new MeasureType[3];
            _DefaultStartPoint[0] = 100;
            _DefaultStartPoint[1] = 200;
            _DefaultStartPoint[2] = 1000; //500 works better

            _CalculatedStartPoint = _DefaultStartPoint;
        }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        ~StartPointCalculator(){
            delete [] _DefaultStartPoint;
        };

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H
