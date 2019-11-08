/*!
 * \file OxModel.h
 * \author Konrad Werys
 * \date 2018/07/29
 */

#ifndef Tomato_OxModel_HXX
#define Tomato_OxModel_HXX

#include "tomatolib_export.h"

namespace Ox {

    /**
     * \class Model
     * \brief Container for a model function, cost function and Least-Squares function. And derivatives.
     * \details Here model function is defined - calcModelValue(). Fitting algorithms based on optimisation need a cost
     * function - calcCostValue(). Fitting algorithms based on least squares need a residuals calculation -
     * calcLSResiduals(). Some fitting algorithms use derivatives, hence calcLSJacobian() and calcCostDerivative().
     * The member variables are pointers to c-arrays, we need to know how many samples we want to process. That's the
     * nSamples defined in the constructor.
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class Model{

    public:

        /**
         * calcModelValue the most important function of this class
         * @param time
         * @return model(time)
         */
        virtual MeasureType calcModelValue(const MeasureType* parameters, MeasureType time) = 0;

        /**
         * calcLSResiduals the most important function of this class
         * @param residuals
         */
        virtual void calcLSResiduals(const MeasureType* parameters, MeasureType* residuals) = 0;

        /**
         * calcCostValue the most important function of this class
         * @return
         */
        virtual MeasureType calcCostValue(const MeasureType* parameters) = 0;

        /**
         * calcCostDerivative the most important function of this class
         * @param derivative
         */
        virtual void calcCostDerivative(const MeasureType* parameters, MeasureType* derivative) = 0;

        /**
         * calcLSJacobian the most important function of this class
         * @param jacobian - 2d matrix stored as 1d array
         */
        virtual void calcLSJacobian(const MeasureType* parameters, MeasureType* jacobian) = 0;

        // getters
        virtual int getNSamples() { return _nSamples; }
        virtual const MeasureType *getInvTimes() const { return _InvTimes; }
        virtual const MeasureType *getEchoTimes() const { return _EchoTimes; }
        virtual const MeasureType *getRepTimes() const { return _RepTimes; }
        virtual const MeasureType *getRelAcqTimes() const { return _RelAcqTimes; }
        virtual const MeasureType *getSignal() const { return _Signal; }
        virtual int getNDims() { return _nDims; }

        // setters
        void setNSamples(int _nSamples);
        virtual void setInvTimes(const MeasureType *_InvTimes) { Model::_InvTimes = _InvTimes; }
        virtual void setEchoTimes(const MeasureType *_EchoTimes) { Model::_EchoTimes = _EchoTimes; }
        virtual void setRepTimes(const MeasureType *_RepTimes) { Model::_RepTimes = _RepTimes; }
        virtual void setRelAcqTimes(const MeasureType *_RelAcqTimes) { Model::_RelAcqTimes = _RelAcqTimes; }
        virtual void setSignal(const MeasureType *_Signal) { Model::_Signal = _Signal; }

        /**
         * \brief show me your ModelT1
         */
        virtual void disp();

        /**
         * \brief set all the pointers to zero
         */
        void setAllPointersToNull();

        /**
         * \brief constructor
         */
        Model();

        /**
         * \brief copy constructor keeps only _nSamples and _nDims
         * @param old
         */
        Model(const Model &old);

        /**
         * cloning
         * @return
         */
        virtual Model<MeasureType> *newByCloning() = 0;

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~Model();

    protected:

        const MeasureType* _InvTimes;
        const MeasureType* _EchoTimes;
        const MeasureType* _RepTimes;
        const MeasureType* _RelAcqTimes;
        const MeasureType* _Signal;
        int _nSamples;
        int _nDims;

        // helper variables
        MeasureType* _Residuals;
    };
} //namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "OxModel.hxx"
#endif //TOMATOLIB_COMPILED

#endif //Tomato_OxModel_H
