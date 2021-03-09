/*!
 * \file OxCalculator.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OXCALCULATORT1_H
#define Tomato_OXCALCULATORT1_H

#include "tomatolib_export.h"
#include "OxFitter.h"
#include "OxModel.h"
#include "OxSignCalculator.h"
#include "OxStartPointCalculator.h"
#include <map>

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

        virtual std::map <std::string, MeasureType> getResults() const;

        /**
         * /throw exception if _Model == 0
         * @return
         */
        virtual  Model<MeasureType> *getModel() const;

        /**
         * /throw exception if _Fitter == 0
         * @return
         */
        virtual Fitter<MeasureType> *getFitter() const;

        virtual StartPointCalculator<MeasureType> *getStartPointCalculator() const;

        virtual SignCalculator<MeasureType> *getSignCalculator() const;

        virtual const MeasureType *getInvTimes() const;

        virtual const MeasureType *getEchoTimes() const;

        virtual const MeasureType *getRepTimes() const;

        virtual const MeasureType *getRelAcqTimes() const;

        /**
         * /throw exception if _SigMag == 0
         * @return
         */
        virtual const MeasureType *getSigMag() const;

        /**
         * does not have to be set
         * @return SigPha pointer, can be 0 (NULL)
         */
        virtual const MeasureType *getSigPha() const;

        virtual const MeasureType *getNoise() const;

        virtual MeasureType *getSignal() const;

        virtual MeasureType *getSigns() const;

        virtual MeasureType *getStartPoint() ;

        virtual MeasureType getMeanCutOff() const;

        /**
          * /throw exception if _nSamples == 0
          * @return
          */
        virtual int getNSamples() const;

        /**
          * /throw exception if _nDims == 0
          * @return
          */
        virtual int getNDims() const;

        MeasureType *getParametersAfterFitting() const;

        /* ****************** */
        /* ***  SETTERS   *** */
        /* ****************** */
        /* ****************** */

        // setters for the 'has a' classes
        virtual void setModel(Model<MeasureType> *_Model);

        virtual void setFitter(Fitter<MeasureType> *_Fitter);

        virtual void setSignCalculator(SignCalculator<MeasureType> *_SignCalculator);

        virtual void setStartPointCalculator(StartPointCalculator<MeasureType> *_StartPointCalculator);

        // setters for the data
        virtual void setInvTimes(const MeasureType *_InvTimes);

        virtual void setEchoTimes(const MeasureType *_EchoTimes);

        virtual void setSigMag(const MeasureType *_SigMag);

        virtual void setSigPha(const MeasureType *_SigPha);

        virtual void setNoise(const MeasureType *_Noise);

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
            _Noise = 0;
            _Signal = 0; // we will be working with this one
            _Signs = 0;  // we will be working with this one
            _StartPoint = 0;
            _ParametersAfterFitting = 0;
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
            delete [] _ParametersAfterFitting; _ParametersAfterFitting = 0;
         };

    protected:

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
        const MeasureType* _Noise;
        MeasureType* _Signal; // size: nSamples. We will be working with this one
        MeasureType* _Signs;  // size: nSamples. We will be working with this one
        MeasureType* _StartPoint; // size: nDims. We will be working with this one

        int _nSamples;
        int _nDims;
        MeasureType _MeanCutOff;

        MeasureType* _ParametersAfterFitting; // size: nDims
        std::map <std::string, MeasureType> _Results;

    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxCalculator.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OXCALCULATORT1_H
