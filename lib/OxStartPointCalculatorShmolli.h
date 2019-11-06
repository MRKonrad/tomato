/*!
 * \file OxStartPointCalculatorShmolli.h
 * \author Konrad Werys
 * \date 2018/08/10
 */

#ifndef Tomato_OXSTARTPOINTCALCULATORShmolli_H
#define Tomato_OXSTARTPOINTCALCULATORShmolli_H

#include "OxStartPointCalculator.h"

namespace Ox {

    /**
     * \class StartPointCalculatorShmolli
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class StartPointCalculatorShmolli : public StartPointCalculator<MeasureType>{

    public:

        /**
         * the most important function of this class
         * @return same as calculateStartPointSKP
         */
        virtual int calculateStartPoint(){
            return calculateStartPointSKP(
                    this->_nSamples,
                    this->_InvTimes,
                    this->_SigMag,
                    this->_Signs,
                    this->_CalculatedStartPoint);
        };

        int setStartPointToDefault(){
            this->_CalculatedStartPoint[0] = this->_DefaultStartPoint[0];
            this->_CalculatedStartPoint[1] = this->_DefaultStartPoint[1];
            this->_CalculatedStartPoint[2] = this->_DefaultStartPoint[2];

            return 0; // EXIT_SUCCESS
        }

        int calculateStartPointSKP(int nSamples, const MeasureType *invTimes, const MeasureType *ysignalInput, const double *signs, MeasureType *initPoint){

            const int MAX_MOLLI_TI_SAMPLES = 128;
            unsigned int i; // gen index
            int lSigns[MAX_MOLLI_TI_SAMPLES] ; // signs local. Always will have first sample inverted and patches the unknown signs from magnitude info
            MeasureType lSignal[MAX_MOLLI_TI_SAMPLES] ; // signed local copy of signal

            // KW: !!!!!
            MeasureType lSignalAbs[MAX_MOLLI_TI_SAMPLES];
            for ( i = 0; i < nSamples; i++) lSignalAbs[i] = fabs(ysignalInput[i]);

            // thse acalcs are absed on ansolute sample valeus
            //a[0]=1.+ 0.5*(y[0]+y[n-1]); //average shortest and longest TI -- poor performance use max
            initPoint[0] = 1.+ KWUtil::max(lSignalAbs[0],lSignalAbs[nSamples-1]); //average shortest and longest TI -- poor performance use max
            initPoint[1] = 1+initPoint[0]+lSignalAbs[0]; //B param, span is a plus absolute of sample with earliest TI
            // patch signs, converts signal to signed values and selsct samples that are over 10% of signal range from A[0]
            int indmin = 0;
            MeasureType signalMin = lSignalAbs[0], singalMax = lSignalAbs[0]; //for signed samples
            KWUtil::MOLLI_min(lSignalAbs, nSamples, &indmin);// find smallest sample index
            initPoint[2]=invTimes[indmin]; // T1* estimate in case all else breaks

            for ( i = 0; i < nSamples; i++) {
                lSigns[i]=signs[i];
                if(lSigns[i] == 0) lSigns[i] = ( i <= indmin ) ? (-1) : (1); // assume negative sign if before lowest(i.e. possible zero corss) signal
                lSignal[i] = lSignalAbs[i] * lSigns[i];
                if(lSignal[i] < signalMin) signalMin = lSignal[i];
                if(lSignal[i] > singalMax) singalMax = lSignal[i];
            }

            MeasureType zereproxrange=0.1; // how far samples must be for log T1* approximation // this works best with ShMLLI 3+3 sample perIR tests
            MeasureType y4log[MAX_MOLLI_TI_SAMPLES] ; // mag sample distance from A for log estiamte
            MeasureType x4log[MAX_MOLLI_TI_SAMPLES] ; // corresponding TIs
            int n4log=0; // number of indexes within range suitable to get logs
            //MeasureType sdMolliBestCost= 1e32; // large enough... to prevent overshadow any better solution
            for (i=0;i<nSamples;i++)
            {
                y4log[n4log]=initPoint[0]-lSignal[i];
                if((y4log[n4log]) > ((singalMax-signalMin)*zereproxrange) )
                {
                    y4log[n4log]=log(y4log[n4log]);
                    x4log[n4log]=invTimes[i];
                    n4log++;
                }
            }
            if(n4log > 1){ // perform log slope calculation
                MeasureType rslope, roffset;
                MeasureType r = KWUtil::SKPLinReg(x4log,y4log, n4log, rslope, roffset); // Linear regression , returns correlation coeff
                if( ( r < 0 ) && ( rslope < 0 ))
                {
                    initPoint[2]=-1./rslope;
                    if ((initPoint[2] < (invTimes[0]/3.)) || (initPoint[2]) > (2*invTimes[nSamples-1])) return (1) ; // EXIT_FAILURE something gone wrong.
                    initPoint[2] = KWUtil::min(initPoint[2], invTimes[nSamples-1]); // trim extremes
                    roffset=exp(roffset);
                    initPoint[0] = KWUtil::min(2*initPoint[0],roffset/2);// trim extremes
                    initPoint[1] = KWUtil::min(2*initPoint[1],roffset);// trim extremes
                    return(n4log);
                }
            }
            return(0);
        }

        /**
          * \brief constructor
          */
        StartPointCalculatorShmolli() : StartPointCalculator<MeasureType>(){
            this->setNDims(3);
            _DefaultStartPoint[0] = 100;
            _DefaultStartPoint[1] = 200;
            _DefaultStartPoint[2] = 1000;
        }

        /**
         * \brief copy constructor
         */
        StartPointCalculatorShmolli(const StartPointCalculatorShmolli &old){
            this->setAllPointersToNull();

            _DefaultStartPoint[0] = old._DefaultStartPoint[0];
            _DefaultStartPoint[1] = old._DefaultStartPoint[1];
            _DefaultStartPoint[2] = old._DefaultStartPoint[2];
            this->_nSamples = old._nSamples;
            this->setNDims(old._nDims);
        };

        /**
         * cloning
         * @return
         */
        virtual StartPointCalculator<MeasureType> *newByCloning() { return new StartPointCalculatorShmolli<MeasureType>(*this); }

        /**
         * \brief do not forget about the virtual destructor, see
         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
         */
        virtual ~StartPointCalculatorShmolli(){};

    protected:
        MeasureType _DefaultStartPoint[3];

    };
} //namespace Ox

#endif //Tomato_OXStartPointCalculatorShmolli_H
