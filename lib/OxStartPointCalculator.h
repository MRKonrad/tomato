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

        // getters
        MeasureType *getCalculatedStartPoint() const {
            if (!_CalculatedStartPoint) {
                std::cerr << "_CalculatedStartPoint equals 0. Set _CalculatedStartPoint" << std::endl;
                throw std::exception();
            };
            return _CalculatedStartPoint;
        }

        const MeasureType *getDefaultStartPoint() const {
            return _DefaultStartPoint;
        }

        int getNStartFitPoints() const { return _nStartFitPoints; }

        //setters
        virtual void setInvTimes(const MeasureType *_InvTimes) { StartPointCalculator::_InvTimes = _InvTimes; }
        virtual void setSignal(const MeasureType *_Signal) { StartPointCalculator::_Signal = _Signal; }
        virtual void setSigns(const MeasureType *_Signs) { StartPointCalculator::_Signs = _Signs; }
        virtual void setCalculatedStartPoint(MeasureType *_CalculatedStartPoint) { StartPointCalculator::_CalculatedStartPoint = _CalculatedStartPoint; }
        virtual void setNSamples(int _nSamples) { StartPointCalculator::_nSamples = _nSamples; }

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculateStartPoint(){
            setStartPointToDefault();
            return 0; // EXIT_SUCCESS
        };

        int setStartPointToDefault(){
            getCalculatedStartPoint()[0] = _DefaultStartPoint[0];
            getCalculatedStartPoint()[1] = _DefaultStartPoint[1];
            getCalculatedStartPoint()[2] = _DefaultStartPoint[2];

            return 0; // EXIT_SUCCESS
        }

        /**
          * \brief default constructor
          */
        StartPointCalculator(){
            _DefaultStartPoint[0] = 100;
            _DefaultStartPoint[1] = 200;
            _DefaultStartPoint[2] = 1000;

            _CalculatedStartPoint = 0;
            _InvTimes = 0;
            _Signal = 0;
            _Signs = 0;
            _nSamples = 0;
            _nStartFitPoints = 3;
        }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculator(){};

    protected:
        MeasureType _DefaultStartPoint[3];
        MeasureType* _CalculatedStartPoint;

        const MeasureType* _InvTimes;
        const MeasureType* _Signal;
        const MeasureType* _Signs;

        int _nSamples;
        int _nStartFitPoints; // special ShMOLLI parameter

    };
} //namespace Ox

#endif //OXSHMOLLI2_OXSTARTPOINTCALCULATOR_H
