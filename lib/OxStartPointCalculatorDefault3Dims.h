/*!
 * \file OxStartPointCalculatorDefault3Dims.h
 * \author Konrad Werys
 * \date 2018/08/10
 */

#ifndef Tomato_OXSTARTPOINTCALCULATORDefault3Dims_H
#define Tomato_OXSTARTPOINTCALCULATORDefault3Dims_H

#include "OxStartPointCalculator.h"

namespace Ox {

    /**
     * \class StartPointCalculatorDefault3Dims
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class StartPointCalculatorDefault3Dims : public StartPointCalculator<MeasureType>{

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
            this->getCalculatedStartPoint()[2] = _DefaultStartPoint[2];

            return 0; // EXIT_SUCCESS
        }

        /**
          * \brief constructor
          */
        StartPointCalculatorDefault3Dims() : StartPointCalculator<MeasureType>(){
            this->_nDims = 3;

            _DefaultStartPoint[0] = 100;
            _DefaultStartPoint[1] = 200;
            _DefaultStartPoint[2] = 1000;

        }

        /**
         * \brief copy constructor
         */
        StartPointCalculatorDefault3Dims(const StartPointCalculatorDefault3Dims &old){
            this->setAllPointersToNull();

            _DefaultStartPoint[0] = old._DefaultStartPoint[0];
            _DefaultStartPoint[1] = old._DefaultStartPoint[1];
            _DefaultStartPoint[2] = old._DefaultStartPoint[2];
            this->_nSamples = old._nSamples;
            this->_nDims = old._nDims;
        };

        /**
         * cloning
         * @return
         */
        virtual StartPointCalculator<MeasureType> *newByCloning() { return new StartPointCalculatorDefault3Dims<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculatorDefault3Dims(){};

    protected:
        MeasureType _DefaultStartPoint[3];

    };
} //namespace Ox

#endif //Tomato_OXStartPointCalculatorDefault3Dims_H
