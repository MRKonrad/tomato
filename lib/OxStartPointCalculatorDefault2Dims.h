/*!
 * \file OxStartPointCalculatorDefault2Dims.h
 * \author Konrad Werys
 * \date 2018/08/10
 */

#ifndef Tomato_OXSTARTPOINTCALCULATORDefault2Dims_H
#define Tomato_OXSTARTPOINTCALCULATORDefault2Dims_H

#include "OxStartPointCalculator.h"

namespace Ox {

    /**
     * \class StartPointCalculatorDefault2Dims
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class StartPointCalculatorDefault2Dims : public StartPointCalculator<MeasureType>{

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateStartPoint(){
            setStartPointToDefault();
            return 0; // EXIT_SUCCESS
        };

        int setStartPointToDefault(){
            this->getCalculatedStartPoint()[0] = _DefaultStartPoint[0];
            this->getCalculatedStartPoint()[1] = _DefaultStartPoint[1];

            return 0; // EXIT_SUCCESS
        }

        /**
          * \brief constructor
          */
        StartPointCalculatorDefault2Dims() : StartPointCalculator<MeasureType>(){
            this->_nDims = 2;

            _DefaultStartPoint[0] = 100;
            _DefaultStartPoint[1] = 1000;

        }

        /**
         * \brief copy constructor
         */
        StartPointCalculatorDefault2Dims(const StartPointCalculatorDefault2Dims &old){
            this->setAllPointersToNull();

            _DefaultStartPoint[0] = old._DefaultStartPoint[0];
            _DefaultStartPoint[1] = old._DefaultStartPoint[1];
            this->_nSamples = old._nSamples;
            this->_nDims = old._nDims;
        };

        /**
         * cloning
         * @return
         */
        virtual StartPointCalculator<MeasureType> *newByCloning() { return new StartPointCalculatorDefault2Dims<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculatorDefault2Dims(){};

    protected:
        MeasureType _DefaultStartPoint[2];

    };
} //namespace Ox

#endif //Tomato_OXStartPointCalculatorDefault2Dims_H
