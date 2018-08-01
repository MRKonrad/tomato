/*!
 * \file OxStartPointCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
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

        //setters
        virtual void setInvTimes(const MeasureType *_InvTimes) { StartPointCalculator::_InvTimes = _InvTimes; }
        virtual void setSignal(MeasureType *_Signal) { StartPointCalculator::_Signal = _Signal; }
        virtual void setSigns(MeasureType *_Signs) { StartPointCalculator::_Signs = _Signs; }
        virtual void setParameters(MeasureType *_Parameters) { StartPointCalculator::_Parameters = _Parameters; }
        virtual void setNSamples(int _nSamples) { StartPointCalculator::_nSamples = _nSamples; }

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateStartPoint(){

            _CalculatedStartPoint[0] = _DefaultStartPoint[0];
            _CalculatedStartPoint[1] = _DefaultStartPoint[1];
            _CalculatedStartPoint[2] = _DefaultStartPoint[2];

            return 0; // EXIT_SUCCESS
        };

        /**
          * \brief default constructor
          */
        StartPointCalculator(){
            _CalculatedStartPoint = 0;
            _Parameters = 0;
            _InvTimes = 0;
            _Signal = 0;
            _Signs = 0;
            _nSamples = 0;
        }

    protected:
        MeasureType _DefaultStartPoint[3] = {100, 200, 100};

        MeasureType* _CalculatedStartPoint;
        const MeasureType* _Parameters;
        const MeasureType* _InvTimes;
        const MeasureType* _Signal;
        const MeasureType* _Signs;
        int _nSamples;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculator(){};

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H
