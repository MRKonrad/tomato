% To use tomato you need two things:
% 1. tomato executable
% 2. configuration file in yaml. 
%	a. yamlmatlab allows you to read yaml to a structure and write a
%	structure to yaml
%   b. the best way to start with the config files is to read a template
%   yaml file
%   c. define/modify parameters in the structure
%   d. save structure to a yaml file
% Then, using matlab 'system' command you can run tomato from matlab. Voila!
%
% Hint: look at the output of the tomato executable to see what options are
% available in the yaml config file
%
% Konrad Werys 2018
% konrad.werys@cardiov.ox.ac.uk

clear 

%% 1. what is the path to the tomato exacutable?
% exepath = 'TomatoExe_v0.1_Windows_x64.exe'; % downloaded from github on windows
exepath = '../cmake-build-release/TomatoExe'; % build from clion on macos

%% 2.a. add yaml reading and writing lib to path
addpath(genpath('../thirdparty/yamlmatlab'));

%% 2.b. the best way to start with the config files is to read a template yaml file and modify it (2.c.)
template_yaml_file = '../tests/testData/Hcmr_Phantom_1916_Shmolli_192i_e11_inputDirs.yaml';
% template_yaml_file = '../tests/testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml';
YamlStruct = yaml.ReadYaml(template_yaml_file);

%% 2.c. define/modify params in the structure. Input params are obligatory for calculation, the rest are optional. 

% %%% input params %%%
YamlStruct.files_magnitude = '';
YamlStruct.files_phase = '';
YamlStruct.dir_magnitude = '../tests/testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12';
YamlStruct.dir_phase = '../tests/testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13';

% % %%% output params %%%
% YamlStruct.dir_output_map = 'map';
% YamlStruct.dir_output_fitparams = 'fitparams';
% YamlStruct.output_map_series_number = 9998;
% YamlStruct.output_fitparams_series_number = 9999;
% 
% %%% calculation params %%%
% YamlStruct.parameter_to_map = 'T1_MOLLI';
% YamlStruct.fitting_method = 'LevMarVnl';
% YamlStruct.functions_type = 'FunctionsBasic';
% YamlStruct.sign_calc_method = 'NoSign';
% YamlStruct.start_point_calc_method = 'Default';
% YamlStruct.fTolerance = 1.0E-12;
% YamlStruct.max_function_evals = 4000.0;
% YamlStruct.use_gradient = 0;
% YamlStruct.mean_cut_off = 10;
% YamlStruct.number_of_threads = 0;
% 
% %%% visualise params %%%
% YamlStruct.visualise = 1;

%% 2.d. save structure to a yaml file
my_yaml_file = './my_yaml_file.yaml';
yaml.WriteYaml(my_yaml_file, YamlStruct, 0);

%% Run tomato
system([exepath, ' ', my_yaml_file]);
