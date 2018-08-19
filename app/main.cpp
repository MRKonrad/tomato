/*!
 * \file main.cpp
 * \author Konrad Werys
 * \date 2018/07/24
 */

#include <iostream>
#include "OxShmolli2.h"
#include "OxShmolli2Options.h"

/**
 * main
 * @return always 0
 */
int main(int argc, char* argv[]) {

    if (argc != 2){
        printf("\nUse: OxShmolliExe(<input_file.yaml>). Please see examples in testData\n");
        Ox::OxShmolli2Options<int>::printAvailable();
    } else if (argc == 2){
        std::string inputFileName(argv[1]);
        sillyFunction(inputFileName);
    }

    printf("Thank you for using!\n");
    printf("Konrad Werys 2018\n");
    printf("konradwerys2@gmail.com\n\n");

    return 0;
}