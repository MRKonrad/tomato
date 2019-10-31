/*!
 * \file main.cpp
 * \author Konrad Werys
 * \date 2018/07/24
 */

#include <iostream>

#include "CmakeConfigForTomato.h"
#include "Tomato.h"
#include "TomatoOptions.h"

/**
 * main
 * Try tests/testData/Hcmr_Phantom_1916_Shmolli_192i_e11_fileList.yaml
 * @return always 0
 */
int main(int argc, char* argv[]) {

    if (argc != 2){
        printf("\nUse: TomatoExe(<input_file.yaml>). Please see an example .yaml file in testData folder. Below all the possible options are listed.\n");
        Ox::TomatoOptions<double> opts;
        opts.printCurrent();
    }
    else if (argc == 2) {

        // process only if ITK is available
#ifdef USE_ITK
        std::string inputFileName(argv[1]);
        Ox::Tomato<double> Tomato_object(inputFileName);
        Tomato_object._opts->printCurrent();
        Tomato_object.readAndSort();
        Tomato_object.calculate();
        Tomato_object.exportToDicom();
        Tomato_object._opts->exportToYaml();

#else
        throw std::runtime_error("You need ITK to run the executable");
#endif

    }

    printf("\nThank you for using tomato v%d.%d\n", Tomato_VERSION_MAJOR, Tomato_VERSION_MINOR);
    printf("Konrad Werys 2019\n");
    printf("konrad.werys@cardiov.ox.ac.uk\n\n");

    return 0;
}

