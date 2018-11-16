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

#include "OxFactoryOfCalculators.h"
#include "OxFactoryOfFitters.h"
#include "OxFactoryOfFunctions.h"
#include "OxFactoryOfSignCalculators.h"
#include "OxFactoryOfStartPointCalculators.h"

#include "TomatoParser.h"

#include "KWUtil.h"

namespace Ox {
    template<typename MeasureType>
    struct TomatoOptions {

        // paths
        std::vector<std::string> files_magnitude;
        std::vector<std::string> files_phase;
        std::string dir_magnitude;
        std::string dir_phase;
        std::string dir_output_map;
        std::string dir_output_fitparams;

        calculatorsType_t parameter_to_map;
        fittersType_t fitting_method;
        functionsType_t functions_type;
        signCalculatorsType_t sign_calc_method;
        startPointCalculatorsType_t start_point_calc_method;

        MeasureType fTolerance;
        int max_function_evals;
        bool use_gradient;

        // image
        MeasureType mean_cut_off;
        int number_of_threads;

        // visualisation
        bool visualise;

        // export
        int output_map_series_number;
        int output_fitparams_series_number;

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

            parameter_to_map = T1_MOLLI;
            fitting_method = LevMarVnl;
            functions_type = FunctionsBasic;
            sign_calc_method = NoSign;
            start_point_calc_method = Default;

            fTolerance = 1e-12;
            max_function_evals = 4000;
            use_gradient = false;

            // image
            mean_cut_off = 10;
            number_of_threads = 0;

            // visualisation
            visualise = false;

            // export
            output_map_series_number = 0;
            output_fitparams_series_number = 0;

            // output params
            calculation_time = 0;
        }

        /**
         * constructor with defaults
         */
        TomatoOptions() {
            init();
        }

        /**
         * constructor with parser
         * @param filePath
         */
        TomatoOptions(std::string filePath) {
            init();

            Ox::TomatoParser<MeasureType> parser;
            parser._filePath = filePath;
            parser._sequences["files_magnitude"];
            parser._sequences["files_phase"];
            parser._scalars["dir_magnitude"];
            parser._scalars["dir_phase"];
            parser._scalars["dir_output_map"];
            parser._scalars["dir_output_fitparams"];

            parser._scalars["parameter_to_map"];
            parser._scalars["fitting_method"];
            parser._scalars["functions_type"];
            parser._scalars["sign_calc_method"];
            parser._scalars["start_point_calc_method"];

            parser._scalars["fTolerance"];
            parser._scalars["max_function_evals"];
            parser._scalars["use_gradient"];

            parser._scalars["mean_cut_off"];
            parser._scalars["number_of_threads"];
            parser._scalars["visualise"];

            parser._scalars["output_map_series_number"];
            parser._scalars["output_fitparams_series_number"];

            parser.parse();

            // input
            files_magnitude = parser._sequences["files_magnitude"];
            files_phase = parser._sequences["files_phase"];
            dir_magnitude = parser._scalars["dir_magnitude"];
            dir_phase = parser._scalars["dir_phase"];

            // output
            dir_output_map = parser._scalars["dir_output_map"];
            dir_output_fitparams = parser._scalars["dir_output_fitparams"];

            if (!parser._scalars["output_map_series_number"].empty())
                output_map_series_number = KWUtil::StringToNumber<MeasureType>(parser._scalars["output_map_series_number"]);
            if (!parser._scalars["output_fitparams_series_number"].empty())
                output_fitparams_series_number = KWUtil::StringToNumber<MeasureType>(parser._scalars["output_fitparams_series_number"]);

            // calc options
            if (!parser._scalars["parameter_to_map"].empty())
                parameter_to_map = (calculatorsType_t) findInArray(lastCalculatorType+1, calculatorsTypeNames, parser._scalars["parameter_to_map"]);
            if (!parser._scalars["fitting_method"].empty())
                fitting_method = (fittersType_t) findInArray(lastFitterType+1, fittersTypeNames, parser._scalars["fitting_method"]);
            if (!parser._scalars["functions_type"].empty())
                functions_type = (functionsType_t) findInArray(lastFunctorType+1, functionsTypeNames, parser._scalars["functions_type"]);
            if (!parser._scalars["sign_calc_method"].empty())
                sign_calc_method = (signCalculatorsType_t) findInArray(lastSignCalculatorType+1, signCalculatorsTypeNames, parser._scalars["sign_calc_method"]);
            if (!parser._scalars["start_point_calc_method"].empty())
                start_point_calc_method = (startPointCalculatorsType_t) findInArray(lastStartPointCalculatorType+1, startPointCalculatorsTypeNames, parser._scalars["start_point_calc_method"]);

            if (!parser._scalars["fTolerance"].empty())
                fTolerance = KWUtil::StringToNumber<MeasureType>(parser._scalars["fTolerance"]);
            if (!parser._scalars["max_function_evals"].empty())
                max_function_evals = KWUtil::StringToNumber<MeasureType>(parser._scalars["max_function_evals"]);
            if (!parser._scalars["use_gradient"].empty())
                use_gradient = (bool)KWUtil::StringToNumber<MeasureType>(parser._scalars["use_gradient"]);

            if (!parser._scalars["mean_cut_off"].empty())
                mean_cut_off = KWUtil::StringToNumber<MeasureType>(parser._scalars["mean_cut_off"]);
            if (!parser._scalars["number_of_threads"].empty())
                number_of_threads = KWUtil::StringToNumber<MeasureType>(parser._scalars["number_of_threads"]);

            // visualise
            if (!parser._scalars["visualise"].empty())
                visualise = (bool)KWUtil::StringToNumber<MeasureType>(parser._scalars["visualise"]);


        }

        int findInArray(int size, const char *nameArray[], std::string name){
            for (int i = 0; i < size; ++i){
                std::string nameFromArray(nameArray[i]);
                if (nameFromArray.compare(name) == 0){
                    return i;
                }
            }
            std::string errorMessage = "Option \"" + name + "\" not found";
            throw std::runtime_error(errorMessage);
        }

        static void printArray(int size, const char *nameArray[]){
            for (int i = 0; i < size; ++i){
                printf("%s ", nameArray[i]);
            }
        }

        template<typename TYPE>
        static void printVector(std::string name, std::vector<TYPE> vector){
            std::cout << name << std::endl;
            for (int i = 0; i < vector.size(); ++i){
                std::cout << "  " << vector.at(i) << std::endl;
            }
        }

        void printCurrent() {
            printf("\n");
            printf("######################################\n");
            printf("Tomato CURRENTLY SELECTED options:\n");
            printf("######################################\n");
            printVector(" files_magnitude: ", files_magnitude);
            printVector(" files_phase: ", files_phase);
            printf(" dir_magnitude: %s ", dir_magnitude.c_str());
            printf("\n dir_phase: %s ", dir_phase.c_str());
            printf("\n dir_output_map: %s ", dir_output_map.c_str());
            printf("\n dir_output_fitparams: %s ", dir_output_fitparams.c_str());
            printf("\n");
            printf("\n parameter_to_map: %s [ ", calculatorsTypeNames[parameter_to_map]);
            printArray(lastCalculatorType+1, calculatorsTypeNames);
            printf("]\n fitting_method: %s [ ",  fittersTypeNames[fitting_method]);
            printArray(lastFitterType+1, fittersTypeNames);
            printf("]\n functions_type: %s [ ", functionsTypeNames[functions_type]);
            printArray(lastFunctorType+1, functionsTypeNames);
            printf("]\n sign_calc_method: %s [ ", signCalculatorsTypeNames[sign_calc_method]);
            printArray(lastSignCalculatorType+1, signCalculatorsTypeNames);
            printf("]\n start_point_calc_method: %s [ ", startPointCalculatorsTypeNames[start_point_calc_method]);
            printArray(lastStartPointCalculatorType+1, startPointCalculatorsTypeNames);
            printf("]\n");
            printf("\n fTolerance: %.2e ", fTolerance);
            //printf("xTolerance: %.2e ", xTolerance);
            printf("\n max_function_evals: %d ", max_function_evals);
            printf("\n use_gradient: %s", use_gradient?"1":"0");
            printf("\n mean_cut_off: %.2f ", mean_cut_off);
            //printf("fittingCutOff: %.2f ", fittingCutOff);
            printf("\n number_of_threads: %d", number_of_threads);
            printf("\n visualise: %s", visualise?"1":"0");

            printf("\n output_map_series_number: %d", output_map_series_number);
            printf("\n output_fitparams_series_number: %d", output_fitparams_series_number);

            printf("\n\n");
        }

        static void printAvailable() {
            printf("\n");
            printf("#############################\n");
            printf("Tomato AVAILABLE options:\n");
            printf("#############################\n");
            printf(" parameter_to_map: ");
            printArray(lastCalculatorType+1, calculatorsTypeNames);
            printf("\n fitting_method: ");
            printArray(lastFitterType+1, fittersTypeNames);
            printf("\n functions_type: ");
            printArray(lastFunctorType+1, functionsTypeNames);
            printf("\n sign_calc_method: ");
            printArray(lastSignCalculatorType+1, signCalculatorsTypeNames);
            printf("\n start_point_calc_method: ");
            printArray(lastStartPointCalculatorType+1, startPointCalculatorsTypeNames);

            printf("\n\n");
        }
    };
} // namespace Ox

#endif //Tomato_TomatoOPTIONS_H
