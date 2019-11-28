/*!
 * \file TomatoColormap.h
 * \author Konrad Werys
 * \date 2017/08/23
 */

#ifndef TomatoLIB_SHMOLLICOLORMAP_H
#define TomatoLIB_SHMOLLICOLORMAP_H

#include "CmakeConfigForTomato.h"

// TODO: reading from a file

#include <vector>
#include <string>
#include <iostream>

#ifdef USE_ITK

std::string getEncodedSting(std::vector < unsigned short > encodeSource);

std::vector < unsigned short > getShmolliColormapRed3TArray();
std::vector < unsigned short > getShmolliColormapGreen3TArray();
std::vector < unsigned short > getShmolliColormapBlue3TArray();

std::vector < unsigned short > getShmolliColormapRed15TArray();
std::vector < unsigned short > getShmolliColormapGreen15TArray();
std::vector < unsigned short > getShmolliColormapBlue15TArray();


#endif //USE_ITK

#endif //TomatoLIB_SHMOLLICOLORMAP_H
