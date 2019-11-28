/*!
 * \file TomatoOnePixel.h
 * \author Konrad Werys
 * \date 2019/11/26
 */

#ifndef TOMATO_TOMATOONEPIXEL_H
#define TOMATO_TOMATOONEPIXEL_H

#include "TomatoOptions.h"

namespace Ox {
    template<typename MeasureType>
    std::map<std::string, MeasureType> calculateOnePixel(TomatoOptions<MeasureType> opts) {

        // alloc and init
        Calculator<MeasureType> *calculator = FactoryOfCalculators<MeasureType>::newByFactory(&opts);
        Model<MeasureType> *model = FactoryOfModels<MeasureType>::newByFactory(&opts);
        Fitter<MeasureType> *fitter = FactoryOfFitters<MeasureType>::newByFactory(&opts);
        SignCalculator<MeasureType> *signCalculator = FactoryOfSignCalculators<MeasureType>::newByFactory(&opts);
        StartPointCalculator<MeasureType> *startPointCalculator = FactoryOfStartPointCalculators<MeasureType>::newByFactory(&opts);

        if (opts.signal_magnitude.size() > 0) {
            calculator->setNSamples(opts.signal_magnitude.size());
            calculator->setSigMag(&(opts.signal_magnitude)[0]);
        } else {
            throw std::runtime_error("\nNo magnitude signal, I cannot calculate anything");
        }

        if (opts.signal_phase.size() > 0) {
            calculator->setSigMag(&(opts.signal_phase)[0]);
        }

        if (opts.noise.size() > 0) {
            calculator->setNoise(&(opts.noise)[0]);
        }

        if (opts.inversion_times.size() > 0) {
            calculator->setInvTimes(&(opts.inversion_times)[0]);
        }

        if (opts.echo_times.size() > 0) {
            calculator->setEchoTimes(&(opts.echo_times)[0]);
        }

        // configure calculator
        calculator->setModel(model);
        calculator->setFitter(fitter);
        calculator->setSignCalculator(signCalculator);
        calculator->setStartPointCalculator(startPointCalculator);

        calculator->calculate();

        std::map<std::string, MeasureType> results = calculator->getResults();

        std::cout << "Results: \n";
        for (typename std::map<std::string, MeasureType>::const_iterator it = results.begin(); it != results.end(); ++it) {
            std::cout << it->first << " " << it->second << "\n";
        }
        std::cout << std::endl;

        delete model;
        delete fitter;
        delete signCalculator;
        delete startPointCalculator;
        delete calculator;

        return results;
    }
} // namespace Ox

#endif //TOMATO_TOMATOONEPIXEL_H
