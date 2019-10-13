# Different results from the configurations

parameter_to_map: T1_SHMOLLI  
functions_type: **FunctionsBasic/FunctionsShmolli**  
fitting_method: LevMarVnl  
max_function_evals: 4000  
fTolerance: 1e-12  
sign_calc_method: MagPhase  
start_point_calc_method: Default  

FunctionsBasic: 10s and seemingly correct results  
FunctionsShmolli: 2.2s and wrong results  

parameter_to_map: T1_SHMOLLI  
functions_type: FunctionsShmolli  
fitting_method: LevMarVnl  
max_function_evals: 4000  
fTolerance: 1e-12  
sign_calc_method: MagPhase  
start_point_calc_method: **Default/StartPointSHMOLLI**  

Default: 2.2s and wrong results  
StartPointSHMOLLI: 10s and seemingly correct results

# 5 vs 7 samples

5 samples are calculated way slower. Maybe playing with fmax and maxIter would reduce the time?
