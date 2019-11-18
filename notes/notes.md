# Notes

*   **Continuous integration** was tested [in this repo](https://github.com/MRKonrad/ContinousIntegrationPlayground).
*   **Gitmodules**: after `git clone` run `git submodule update --init --recursive` to get the submodules code.
*   It would be nice to follow [CppCoreGuidlines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) as far as C++98 allows.
*   Maybe I can write a script for github releases with [github releases REST api](https://developer.github.com/v3/repos/releases/#create-a-release) and [curl REST api](http://www.codingpedia.org/ama/how-to-test-a-rest-api-from-command-line-with-curl/#12_HEAD_requests)?
*   LMFIT - somehow windows does not like the static library, so the dynamic is used

## TODO

*   matlab 1 pixel calculation
*   yaml input that gets the start point and the inversion/echo times as array
*   better signCalculatorsTypeNames and startPointCalculatorsTypeNames
*   results - currently a map, and an akward itkCapculatorT1 filter. Maybe there is a better approach to results?
*   provide yaml config files for MOLLI, ShMOLLI, SASHA, SMART1 etc 
*   add acceptance test for T1 2param and T2
*   maybe rename calculatormolli to calculator3param? 
*   maybe add calculator2param to produce different results?
*   update comments in configuration yaml files
*   output SNR maps
*   make sure SNR and nAmoebaCalls is the same as in Shmolli
*   possibly I could make all Model object static