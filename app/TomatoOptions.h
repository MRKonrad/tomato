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

#include "OxFactoryOfCalculators.h"
#include "OxFactoryOfFitters.h"
#include "OxFactoryOfModels.h"
#include "OxFactoryOfSignCalculators.h"
#include "OxFactoryOfStartPointCalculators.h"

#include "TomatoParser.h"

#include "KWUtil.h"
#include "KWUtilYaml.h"

#define YAML_BUFFER_SIZE 65536

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
        paramType_t parameter_type;
        fittersType_t fitting_method;
        modelType_t model_type;
        signCalculatorsType_t sign_calc_method;
        startPointCalculatorsType_t start_point_calc_method;

        MeasureType fTolerance;
        int max_function_evals;
        bool use_gradient;

        // image
        MeasureType mean_cut_off;
        MeasureType map_scale_factor;
        bool use_colorbar;
        int number_of_threads;

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
            parser._scalars["model_type"];
            parser._scalars["sign_calc_method"];
            parser._scalars["start_point_calc_method"];

            parser._scalars["fTolerance"];
            parser._scalars["max_function_evals"];
            parser._scalars["use_gradient"];

            parser._scalars["mean_cut_off"];
            parser._scalars["map_scale_factor"];
            parser._scalars["use_colorbar"];
            parser._scalars["number_of_threads"];

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
                parameter_type = (paramType_t)calculatorsParamsToCalculate[(int)parameter_to_map];
            if (!parser._scalars["fitting_method"].empty())
                fitting_method = (fittersType_t) findInArray(lastFitterType+1, fittersTypeNames, parser._scalars["fitting_method"]);
            if (!parser._scalars["model_type"].empty())
                model_type = (modelType_t) findInArray(lastFunctorType + 1, modelTypeNames, parser._scalars["model_type"]);
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
            if (!parser._scalars["map_scale_factor"].empty())
                map_scale_factor = KWUtil::StringToNumber<MeasureType>(parser._scalars["map_scale_factor"]);
            if (!parser._scalars["use_colorbar"].empty())
                use_colorbar = KWUtil::StringToNumber<MeasureType>(parser._scalars["use_colorbar"]);
            if (!parser._scalars["number_of_threads"].empty())
                number_of_threads = KWUtil::StringToNumber<MeasureType>(parser._scalars["number_of_threads"]);

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

        void printCurrent() {
            printf("\n");
            printf("######################################\n");
            printf("Tomato CURRENTLY SELECTED options:\n");
            printf("######################################\n");
            KWUtil::printVector(" files_magnitude: ", files_magnitude);
            KWUtil::printVector(" files_phase: ", files_phase);
            printf(" dir_magnitude: %s ", dir_magnitude.c_str());
            printf("\n dir_phase: %s ", dir_phase.c_str());
            printf("\n dir_output_map: %s ", dir_output_map.c_str());
            printf("\n dir_output_fitparams: %s ", dir_output_fitparams.c_str());
            printf("\n output_map_series_number: %d", output_map_series_number);
            printf("\n output_fitparams_series_number: %d", output_fitparams_series_number);
            printf("\n");
            printf("\n");
            Ox::FactoryOfCalculators<double>::disp(parameter_to_map);
            Ox::FactoryOfFitters<double>::disp(fitting_method);
            Ox::FactoryOfModels<double>::disp(model_type);
            Ox::FactoryOfSignCalculators<double>::disp(sign_calc_method);
            Ox::FactoryOfStartPointCalculators<double>::disp(start_point_calc_method);

            printf("\n fTolerance: %.2e", fTolerance);
            //printf("xTolerance: %.2e ", xTolerance);
            printf("\n max_function_evals: %d", max_function_evals);
            printf("\n use_gradient: %s", use_gradient ? "1" : "0");
            printf("\n mean_cut_off: %.2f", mean_cut_off);
            printf("\n map_scale_factor: %.2f", map_scale_factor);
            printf("\n use_colorbar: %s", use_colorbar ? "1" : "0");
            //printf("fittingCutOff: %.2f ", fittingCutOff);
            printf("\n number_of_threads: %d", number_of_threads);
            if (calculation_time > 0) {
                printf("\n calculation time: %.2fs", calculation_time);
            }

            printf("\n\n");
        }

        int exportToYaml(){

            if (dir_output_map.length() > 0){
                std::string filePath = dir_output_map + KWUtil::PathSeparator() + "tomato_output_config.yaml";
                exportToYaml(filePath);
            }

            if (dir_output_fitparams.length() > 0){
                std::string filePath = dir_output_fitparams + KWUtil::PathSeparator() + "tomato_output_config.yaml";
                exportToYaml(filePath);
            }

            return 0; // EXIT_SUCCESS
        }

        int exportToYaml(std::string filePath){


            yaml_document_t document;
            yaml_emitter_t emitter;
            size_t written = 0;

            yaml_document_initialize(&document, NULL, NULL, NULL, 1, 1);

            yaml_document_add_mapping(&document, (yaml_char_t*)YAML_DEFAULT_MAPPING_TAG, YAML_ANY_MAPPING_STYLE);
            int mapping_node_number = (int)(document.nodes.top - document.nodes.start);

            KWUtilYaml::addSequence(&document, mapping_node_number, "files_magnitude", files_magnitude);
            KWUtilYaml::addSequence(&document, mapping_node_number, "files_phase", files_phase);
            KWUtilYaml::addMapping(&document, mapping_node_number, "dir_magnitude", dir_magnitude);
            KWUtilYaml::addMapping(&document, mapping_node_number, "dir_phase", dir_phase);
            KWUtilYaml::addMapping(&document, mapping_node_number, "dir_output_map", dir_output_map);
            KWUtilYaml::addMapping(&document, mapping_node_number, "dir_output_fitparams", dir_output_fitparams);
            KWUtilYaml::addMapping(&document, mapping_node_number, "output_map_series_number", KWUtil::NumberToString(output_map_series_number));
            KWUtilYaml::addMapping(&document, mapping_node_number, "output_fitparams_series_number", KWUtil::NumberToString(output_fitparams_series_number));

            KWUtilYaml::addMapping(&document, mapping_node_number, "parameter_to_map", calculatorsTypeNames[parameter_to_map]);
            KWUtilYaml::addMapping(&document, mapping_node_number, "fitting_method", fittersTypeNames[fitting_method]);
            KWUtilYaml::addMapping(&document, mapping_node_number, "model_type", modelTypeNames[model_type]);
            KWUtilYaml::addMapping(&document, mapping_node_number, "sign_calc_method" , signCalculatorsTypeNames[sign_calc_method]);
            KWUtilYaml::addMapping(&document, mapping_node_number, "start_point_calc_method", startPointCalculatorsTypeNames[start_point_calc_method]);

            KWUtilYaml::addMapping(&document, mapping_node_number, "fTolerance", KWUtil::NumberToString(fTolerance));
            KWUtilYaml::addMapping(&document, mapping_node_number, "max_function_evals", KWUtil::NumberToString(max_function_evals));
            KWUtilYaml::addMapping(&document, mapping_node_number, "use_gradient", use_gradient ? "1" : "0");

            KWUtilYaml::addMapping(&document, mapping_node_number, "mean_cut_off", KWUtil::NumberToString(mean_cut_off));
            KWUtilYaml::addMapping(&document, mapping_node_number, "map_scale_factor", KWUtil::NumberToString(map_scale_factor));
            KWUtilYaml::addMapping(&document, mapping_node_number, "use_colorbar", use_colorbar ? "1" : "0");
            KWUtilYaml::addMapping(&document, mapping_node_number, "number_of_threads", KWUtil::NumberToString(number_of_threads));
            KWUtilYaml::addMapping(&document, mapping_node_number, "calculation_time", KWUtil::NumberToString(calculation_time));

            std::string tomato_version = "v" + KWUtil::NumberToString(Tomato_VERSION_MAJOR) + "." + KWUtil::NumberToString(Tomato_VERSION_MINOR);
            KWUtilYaml::addMapping(&document, mapping_node_number, "tomato_version", tomato_version);

            unsigned char buffer[YAML_BUFFER_SIZE+1];
            memset(buffer, 0, YAML_BUFFER_SIZE+1);

            yaml_emitter_initialize(&emitter);
            yaml_emitter_set_output_string(&emitter, buffer, YAML_BUFFER_SIZE, &written);
            yaml_emitter_open(&emitter);
            yaml_emitter_dump(&emitter, &document);
            yaml_emitter_flush(&emitter);
            yaml_emitter_close(&emitter);

            FILE *file = fopen(filePath.c_str(), "wb");
            fprintf(file, "%s", buffer);
            fclose(file);

            return 0; // EXIT_SUCCESS

        }

    };
} // namespace Ox

#undef YAML_BUFFER_SIZE

#endif //Tomato_TomatoOPTIONS_H
