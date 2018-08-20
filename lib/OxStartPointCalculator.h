/*!
 * \file OxStartPointCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H
#define OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H

#include "KWUtil.h"

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

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateStartPoint() = 0;

        // getters
        MeasureType *getCalculatedStartPoint() const {
            if (!_CalculatedStartPoint) throw std::runtime_error("_CalculatedStartPoint equals 0. Set _CalculatedStartPoint");
            return _CalculatedStartPoint;
        }

        int getNDims() const { return _nDims; }

        //setters
        virtual void setInvTimes(const MeasureType *_InvTimes) { StartPointCalculator::_InvTimes = _InvTimes; }
        virtual void setSignal(const MeasureType *_Signal) { StartPointCalculator::_Signal = _Signal; }
        virtual void setSigns(const MeasureType *_Signs) { StartPointCalculator::_Signs = _Signs; }
        virtual void setCalculatedStartPoint(MeasureType *_CalculatedStartPoint) { StartPointCalculator::_CalculatedStartPoint = _CalculatedStartPoint; }
        virtual void setNSamples(int _nSamples) { StartPointCalculator::_nSamples = _nSamples; }


        void disp(){
            std::cout << "\nTODO: implement StartPointCalculator disp " << this << std::endl;
        }

        /**
         * \brief set all the pointers to zero
         */
        void setAllPointersToNull(){
            _CalculatedStartPoint = 0;
            _InvTimes = 0;
            _Signal = 0;
            _Signs = 0;
        }

        /**
          * \brief constructor
          */
        StartPointCalculator(){

            setAllPointersToNull();

            _nSamples = 0;
            _nDims = 0;
        }

        /**
         * \brief copy constructor
         */
        StartPointCalculator(const StartPointCalculator &old){

            setAllPointersToNull();

            _nSamples = old._nSamples;
            _nDims = old._nDims;
        };

        /**
         * cloning
         * @return
         */
        virtual StartPointCalculator<MeasureType> *newByCloning() = 0;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculator(){};

    protected:
        MeasureType* _CalculatedStartPoint;

        const MeasureType* _InvTimes;
        const MeasureType* _Signal;
        const MeasureType* _Signs;

        int _nSamples;
        int _nDims; // special ShMOLLI parameter

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H
