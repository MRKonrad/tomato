/*!
 * \file OxCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXCALCULATORT1_H
#define Tomato_OXCALCULATORT1_H

#include "tomatolib_export.h"
#include "OxCalculatorT1Results.h"
#include "OxFitter.h"
#include "OxModel.h"
#include "OxSignCalculator.h"
#include "OxStartPointCalculator.h"

#include "KWUtil.h"

namespace Ox {

    /**
     * \class CalculatorT1
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class Calculator{

    public:

        /**
         * the most important function of this class
         * @return success/failure
         */
        virtual int calculate() = 0;

        /**
         * processing before calculate is called
         * @return success/failure
         */
        virtual int prepareToCalculate() = 0;

        /* ****************** */
        /* ***  GETTERS   *** */
        /* ****************** */

        /**
         * /throw exception if _Model == 0
         * @return
         */
        Model<MeasureType> *getModel() const;

        /**
         * /throw exception if _Fitter == 0
         * @return
         */
        Fitter<MeasureType> *getFitter() const;

        /**
         * /throw exception if _StartPointCalculator == 0
         * @return
         */
        StartPointCalculator<MeasureType> *getStartPointCalculator() const;

        /**
         * /throw exception if _SignCalculator == 0
         * @return
         */
        SignCalculator<MeasureType> *getSignCalculator() const;

        /**
         * /throw exception if _InvTimes == 0
         * @return
         */
        const MeasureType *getInvTimes() const;

        const MeasureType *getEchoTimes() const;

        const MeasureType *getRepTimes() const;

        const MeasureType *getRelAcqTimes() const;

        /**
         * /throw exception if _SigMag == 0
         * @return
         */
        const MeasureType *getSigMag() const;

        /**
         * does not have to be set
         * @return SigPha pointer, can be 0 (NULL)
         */
        const MeasureType *getSigPha() const;

        MeasureType *getSignal() const;

        MeasureType *getSigns() const;

        MeasureType * getStartPoint() ;

        const CalculatorT1Results<MeasureType> getResults() const;

        MeasureType getMeanCutOff() const;

        /**
          * /throw exception if _nSamples == 0
          * @return
          */
        int getNSamples() const;

        /**
          * /throw exception if _nDims == 0
          * @return
          */
        int getNDims() const;


        /* ****************** */
        /* ***  SETTERS   *** */
        /* ****************** */

        // setters for the 'has a' classes
        void setModel(Model<MeasureType> *_Model);

        void setFitter(Fitter<MeasureType> *_Fitter);

        void setSignCalculator(SignCalculator<MeasureType> *_SignCalculator);

        void setStartPointCalculator(StartPointCalculator<MeasureType> *_StartPointCalculator);

        // setters for the data
        virtual void setInvTimes(const MeasureType *_InvTimes);

        virtual void setEchoTimes(const MeasureType *_EchoTimes);

        virtual void setSigMag(const MeasureType *_SigMag);

        virtual void setSigPha(const MeasureType *_SigPha);

        virtual void setMeanCutOff(MeasureType _MeanCutOff);

        /**
         * _Signal and _Signs are allocated here!!!
         * @param _nSamples
         */
        virtual void setNSamples(int _nSamples);

        /**
         * _StartPoint is allocated here!!!
         * @param _nDims
         */
        virtual void setNDims(int _nDims);

        /**
         * \brief show me your Model
         */
        void disp();

        /**
         * \brief set all the pointers to zero
         */
        void setAllPointersToNull(){
            // objects
            _Model = 0;
            _Fitter = 0;
            _SignCalculator = 0;
            _StartPointCalculator = 0;

            // arrays
            _InvTimes = 0;
            _EchoTimes = 0;
            _RepTimes = 0;
            _RelAcqTimes = 0;
            _SigMag = 0; // original one
            _SigPha = 0; // original one
            _Signal = 0; // we will be working with this one
            _Signs = 0;  // we will be working with this one
            _StartPoint = 0;
        }

        /**
         * \brief constructor
         */
        Calculator() {

            setAllPointersToNull();

            // primitives
            _MeanCutOff = 0;
            _nSamples = 0;
            _nDims = 0;

        };

        /**
         * \brief copy constructor
         */
        Calculator(const Calculator &old){

            setAllPointersToNull();

            // primitives
            setMeanCutOff(old._MeanCutOff);
            setNSamples(old._nSamples);
            setNDims(old._nDims);
        };

        /**
         * cloning
         * @return
         */
        virtual Calculator<MeasureType> *newByCloning() = 0;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~Calculator(){
            delete [] _Signal; _Signal = 0;
            delete [] _Signs; _Signs = 0;
            delete [] _StartPoint; _StartPoint = 0;
         };

    protected:
        CalculatorT1Results<MeasureType> _Results; // we will be working with this one

        Model<MeasureType>* _Model;
        Fitter<MeasureType>* _Fitter;
        SignCalculator<MeasureType>* _SignCalculator;
        StartPointCalculator<MeasureType>* _StartPointCalculator;

        const MeasureType* _InvTimes;
        const MeasureType* _EchoTimes;
        const MeasureType* _RepTimes;
        const MeasureType* _RelAcqTimes;
        const MeasureType* _SigMag; // original one
        const MeasureType* _SigPha; // original one
        MeasureType* _Signal; // size: nSamples. We will be working with this one
        MeasureType* _Signs;  // size: nSamples. We will be working with this one
        MeasureType* _StartPoint; // size: nDims. We will be working with this one

        int _nSamples;
        int _nDims;
        MeasureType _MeanCutOff;

    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxCalculator.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXCALCULATORT1_H
