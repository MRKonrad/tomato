"""
    To use tomato you need two things:
    1. tomato executable
    2. configuration file in yaml.
      a. import yaml
      b. the best way to start with the config files is to read a template yaml file
      c. define/modify parameters in the structure
      d. save structure to a yaml file
    Then, using python 'os.system' command you can run tomato from python. Voila!

    Hint: look at the output of the tomato executable to see what options are available in the yaml config file

    Konrad Werys 2018
    konrad.werys@cardiov.ox.ac.uk
"""

import os
from ruamel.yaml import YAML # 2.a.

if __name__ == '__main__':

    # ### 1 what is the path to the tomato exacutable?
    # exepath = 'TomatoExe_v0.1_Windows_x64.exe'  # downloaded from github on windows
    exepath = '../cmake-build-release/TomatoExe'  # build from clion on macos

    # ### 2.b. the best way to start with the config files is to read a template yaml file and modify it (2.c.)
    template_yaml_file = '../tests/testData/Hcmr_Phantom_1916_Shmolli_192i_e11_inputDirs.yaml'
    # template_yaml_file = '../tests/testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml'

    with open(template_yaml_file, 'r') as myfile:
        yaml_data = myfile.read()
    yaml = YAML()
    yaml_dict = yaml.load(yaml_data)
    
    # ### 2.c. define/modify params in the structure. Input params are obligatory for calculation, the rest are optional

    # ## input params ##
    yaml_dict['files_magnitude'] = ''
    yaml_dict['files_phase'] = ''
    yaml_dict['dir_magnitude'] = '../tests/testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12'
    yaml_dict['dir_phase'] = '../tests/testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_13'

    # ## output params ##
    # yaml_dict['dir_output_map'] = 'map'
    # yaml_dict['dir_output_fitparams'] = 'fitparams'
    # yaml_dict['output_map_series_number'] = 9998
    # yaml_dict['output_fitparams_series_number'] = 9999

    # ## calculation params ##
    # yaml_dict['parameter_to_map'] = 'T1_MOLLI'
    # yaml_dict['fitting_method'] = 'LevMarVnl'
    # yaml_dict['functions_type'] = 'FunctionsBasic'
    # yaml_dict['sign_calc_method'] = 'NoSign'
    # yaml_dict['start_point_calc_method'] = 'Default'
    # yaml_dict['fTolerance'] = 1.0E-12
    # yaml_dict['max_function_evals'] = 4000
    # yaml_dict['use_gradient'] = 0
    # yaml_dict['mean_cut_off'] = 10
    # yaml_dict['number_of_threads'] = 0

    # $$$ 2.d. save structure to a yaml file
    my_yaml_file = './my_yaml_file.yaml'
    with open(my_yaml_file, 'w') as myfile:
        yaml.dump(yaml_dict, myfile)

    os.system(exepath + ' ' + my_yaml_file)
