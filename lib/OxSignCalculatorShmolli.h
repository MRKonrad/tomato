/*!
 * \file OxSignCalculatorShmolli.h
 * \author Konrad Werys
 * \date 2018/08/01
 */

#ifndef OXSHMOLLI2_OXSIGNCALCULATORSHMOLLI_H
#define OXSHMOLLI2_OXSIGNCALCULATORSHMOLLI_H

#include "OxSignCalculator.h"
#include "KWUtil.h"

namespace Ox {

    /**
     * \class SignCalculatorShmolli
     * \brief
     * \details
     * @tparam MeasureType
     */
    template< typename MeasureType >
    class SignCalculatorShmolli : public SignCalculator<MeasureType> {

    public:
        virtual int calculateSign(){
            return SKPPhase2Signs(
                    this->getNSamples(),
                    this->getInvTimes(),
                    this->getSigMag(),
                    this->getSigPha(),
                    this->getSignal(),
                    this->getSigns());
        };

        static int SKPPhase2Signs(
                int nSamples,
                const MeasureType* invTimes,
                const MeasureType* sigMag,
                const MeasureType* sigPha,
                MeasureType* signal,
                MeasureType* signs);

    protected:
        const static int MAX_MOLLI_TI_SAMPLES = 128;
        const static int MAX_T1_TRESHOLD = 4000;

//        /**
//         * \brief do not forget about the virtual destructor, see
//         * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
//         */
//        virtual ~SignCalculatorShmolli(){};
    };

    template<typename MeasureType>
    int
    SignCalculatorShmolli<MeasureType>
    ::SKPPhase2Signs(
            int nSamples,
            const MeasureType* invTimes,
            const MeasureType* sigMag,
            const MeasureType* sigPha,
            MeasureType* signal,
            MeasureType* signs) {
        // takes all samples sorted from shortest to longest TI, returns 0 if clear problems identified to allow rejection of data based on incosistent signs.
        // note: logic for correction of signs works for ShMOLLI, should work for most basic T1_MOLLI variants too. but not fully tested.
        //std::cout << "a" << std::endl;

        KWUtil::copyArrayToArray(nSamples, signal, sigMag);

        double MaxTIForSignInvert = (MAX_T1_TRESHOLD * 0.67);

        double _phaMin = -4096;
        double _phaMax = 4096;
        double zeroRangeFraction = 0.3; // marks size of uncertaintey of corossover between signs. The signs in this zone will be decided by brute force. 0.1 (few doubts) 1 - all phase info doubtful ,0.3 is about right.
        double PIrange = (_phaMax - _phaMin) / 2; // scale to make one pi=~3.14 phase rotation // KW on dicoms *2
        double PI05 = PIrange * 0.5;
        double PIx2 = PIrange * 2;
        int IsPhaseErrorFound = 0;

        double upmap[MAX_MOLLI_TI_SAMPLES]; // phase normalised
        double PhaseAway[MAX_MOLLI_TI_SAMPLES]; // phase normalised
        //int asign[MAX_MOLLI_TI_SAMPLES] ; // signs estimate
        int nsignspositive = 0;
        int nsignsnegative = 0;

        for (int i = 0; i < nSamples; i++) {
            upmap[i] = fmod(PIx2 + sigPha[i] - sigPha[0], PIx2); // make sure within 2*pi
            PhaseAway[i] = fabs(upmap[i] - PIrange);  // away from positive signs  of first smaple+PI
            PhaseAway[i] = ((PI05 - PhaseAway[i]) /
                            (zeroRangeFraction * PI05)); // =How many times outsede midzone grey area around PI/2
            if (PhaseAway[i] < -1) {
                signs[i] = -1;
                nsignsnegative++;
            }
            else if (PhaseAway[i] > 1) {
                signs[i] = 1;
                nsignspositive++;
            }
            else signs[i] = 0;
            if ((signs[i] < 0) && (invTimes[i] > MaxTIForSignInvert)) // detect obvious pahse problems
            {
                IsPhaseErrorFound = 1;// wrong- found clear engative sign for for long TI
                signs[i] = 1; // corect problem in case recon is atempted, // depends on IsAbortreconOnPhaseErrors
            }
        }
        if ((nsignspositive == 0) || (nsignsnegative == 0)) // suspicious, no positive signs were found
        {
            // basic solution  for (i = 0;i < n;i++)signs[i]=0; // dont bother guessing, leave to brute force...
            /// more complicated.
            // sceanrios
            ////(1) longest TI is longer than MaxTIForSignInvert
            /////////// ==> ALL signs must be POSITIVE.
            ////(2) longest TI is SHORTER than MaxTIForSignInvert
            /////////// (2a) 1st and last 4 samples show NEGATIVE trend  (R(1&3456)=>> All signs NEGATIVE
            /////////// (2b) 1st 4 samples show positive trend, which is greater than last 4 samples  and greater than R(1&3456)=>> definite very short  T1, All samaples POSITIVE
            //////////   (else) set all positive + indicate pahse errors.

            ////(1) longest TI is longer than MaxTIForSignInvert
            /////////// ==> ALL signs must be POSITIVE.
            if (invTimes[nSamples - 1] > MaxTIForSignInvert) {
                for (int i = 0; i < nSamples; i++) signs[i] = 1;
                return (1); // loks like conssitent all positive signs
            }
            ////(2) longest TI is SHORTER than MaxTIForSignInvert
            /////////// (2a) 1st and last 4 samples show NEGATIVE trend  (R(1&3456)=>> All signs NEGATIVE
            double y1andlast4[MAX_MOLLI_TI_SAMPLES];
            double x1andlast4[MAX_MOLLI_TI_SAMPLES];
            double R1andlast4 = 0;
            double dummySl, dummyOff;
            y1andlast4[0] = sigMag[0];
            x1andlast4[0] = invTimes[0];
            for (int i = 0; i < 4; i++)
                y1andlast4[i + 1] = sigMag[nSamples - 1 - i]; // order reversed, does not amtter for regression
            for (int i = 0; i < 4; i++)
                x1andlast4[i + 1] = invTimes[nSamples - 1 - i]; // order reversed, does not matter for regression
            //double MOLLI_LinReg(double *x,double *y, int n, double &rslope, double &roffset)
            R1andlast4 = KWUtil::SKPLinReg(x1andlast4, y1andlast4, 5, dummySl, dummyOff);
            if (R1andlast4 <
                (-0.2)) // !!!note arbitrary threshold below zero toa void noise but not too far to account for curvature. !!!!Not validated but this is digging into noise/saving few pixels, should carry no impact.
            {
                for (int i = 0; i < nSamples; i++)signs[i] = -1;
                return (1); // loks like conssitent all negative signs, with high heart rate
            }
            /////////// (2b) 1st 4 samples show positive trend, which is greater than last 4 samples  and greater than R(1&3456)=>> definite very short  T1, All samaples POSITIVE
            double Rfirst4 = KWUtil::SKPLinReg(invTimes, sigMag, 4, dummySl, dummyOff);
            double Rlast4 = KWUtil::SKPLinReg(invTimes + nSamples - 4,
                                              sigMag + nSamples - 4, 4, dummySl, dummyOff);
            if ((Rfirst4 > Rlast4) && (Rfirst4 > R1andlast4)) {
                for (int i = 0; i < nSamples; i++) signs[i] = 1;
                return (1); // loks like conssitent all positive signs, with high heart rate
            }

            //////////   (else) set all positive + indicate pahse errors.
            for (int i = 0; i < nSamples; i++) signs[i] = 1;
            return (0);
        }
        return (!IsPhaseErrorFound);
    }

} //namespace Ox

#endif //OXSHMOLLI2_OXSIGNCALCULATOR_H
