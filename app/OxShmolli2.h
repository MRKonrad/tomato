/*!
 * \file OxShmolli2.h
 * \author Konrad Werys
 * \date 2018/08/14
 */

#ifndef OXSHMOLLI2_OXSHMOLLI2_H
#define OXSHMOLLI2_OXSHMOLLI2_H

#include "OxShmolli2Options.h"
#include "OxFactoryOfFunctions.h"

int sillyFunction(std::string inputFileName){
    typedef double TYPE;
    Ox::OxShmolli2Options<TYPE> opts;
    opts.printAvailable();
    opts.printCurrent();
    Ox::FunctionsT1<TYPE> *a = Ox::FactoryOfFunctions<TYPE>::generateFunctionsObject(&opts);
    a->disp();
    delete a;
    return 0; // EXIT_SUCCESS
}





#endif //OXSHMOLLI2_OXSHMOLLI2_H
