/*!
 * \file TomatoOptions.h
 * \author Konrad Werys
 * \date 2018/08/18
 */

#ifndef Tomato_TomatoOPTIONS_H
#define Tomato_TomatoOPTIONS_H

#include <iostream>
#include <vector>
#include <string>

#include "CmakeConfigForTomato.h"
#include "tomatolib_export.h"

#include "OxFactoryOfCalculators.h"
#include "OxFactoryOfFitters.h"
#include "OxFactoryOfModels.h"
#include "OxFactoryOfSignCalculators.h"
#include "OxFactoryOfStartPointCalculators.h"

namespace Ox {

    template<typename MeasureType>
    struct TomatoOptions {

    public:
        // paths for dicom file based calculation
        std::vector<std::string> files_magnitude;
        std::vector<std::string> files_phase;
        std::string dir_magnitude;
        std::string dir_phase;
        std::string dir_output_map;
        std::string dir_output_fitparams;
        std::string dir_output_magSignRecov;

        // data for one pixel calculation
        std::vector <MeasureType> signal_magnitude;
        std::vector <MeasureType> signal_phase;

        // calculation types
        calculatorsType_t parameter_to_map;
        paramType_t parameter_type;
        fittersType_t fitting_method;
        modelType_t model_type;
        signCalculatorsType_t sign_calc_method;
        startPointCalculatorsType_t start_point_calc_method;

        // calculation params
        MeasureType fTolerance;
        int max_function_evals;
        bool use_gradient;
        std::vector <MeasureType> inversion_times;
        std::vector <MeasureType> echo_times;
        std::vector <MeasureType> start_point;
        std::vector <MeasureType> noise;

        // image
        MeasureType mean_cut_off;
        MeasureType map_scale_factor;
        bool use_colorbar;
        int number_of_threads;

        // export
        int output_map_series_number;
        int output_fitparams_series_number;
        int output_magSignRecov_series_number;

        // output params
        double calculation_time;

        /**
         * initialise the defaults
         * done this way to work around delegating constructors in cpp98
         */
        void init(){
            dir_magnitude = "";
            dir_phase = "";
            dir_output_map = "";
            dir_output_fitparams = "";
            dir_output_magSignRecov = "";

            parameter_to_map = T1_MOLLI;
            fitting_method = LevMarVnl;
            model_type = _ModelT1ThreeParam;
            sign_calc_method = NoSign;
            start_point_calc_method = Basic;

            fTolerance = 1e-12;
            max_function_evals = 4000;
            use_gradient = false;

            // image
            mean_cut_off = 10.0;
            map_scale_factor = 1.0;
            use_colorbar = true;
            number_of_threads = 0;

            // export
            output_map_series_number = 0;
            output_fitparams_series_number = 0;
            output_magSignRecov_series_number = 0;

            // output params
            calculation_time = 0;
        }

        /**
         * constructor with defaults
         */
        TomatoOptions();

        /**
         * constructor with parser
         * @param filePath
         */
        TomatoOptions(const std::string& filePath);

        /**
         * destructor
         */
        ~TomatoOptions();

        /**
         *
         * @return
         */
        int exportToYaml();

        /**
         *
         * @param filePath
         * @return
         */
        int exportToYaml(std::string filePath);

        /**
         * print current state of the struct
         */
        void printCurrent();

        /**
         * useful in looking for params defined in factories
         * @param size
         * @param nameArray
         * @param name
         * @return
         */
        int findInArray(int size, const char *nameArray[], std::string name);

    private:

        void copyStrVectorToMemberVector(std::vector<std::string> strVector, std::vector<MeasureType> &memberVector);

    };
} // namespace Ox

#ifndef TOMATOLIB_COMPILED
#include "TomatoOptions.hxx"
#endif

#endif //Tomato_TomatoOPTIONS_H
