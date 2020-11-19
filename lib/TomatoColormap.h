/*!
 * \file TomatoColormap.h
 * \author Konrad Werys
 * \date 2017/08/23
 */

#ifndef TomatoLIB_SHMOLLICOLORMAP_H
#define TomatoLIB_SHMOLLICOLORMAP_H

#include "CmakeConfigForTomato.h"

#ifdef USE_ITK

// TODO: reading from a file

#include <vector>
#include <string>
#include <iostream>
#include "tomatolib_export.h"

std::string TOMATOLIB_EXPORT getEncodedSting(std::vector < unsigned short > encodeSource);

std::vector < unsigned short > TOMATOLIB_EXPORT getShmolliColormapRed3TArray();
std::vector < unsigned short > TOMATOLIB_EXPORT getShmolliColormapGreen3TArray();
std::vector < unsigned short > TOMATOLIB_EXPORT getShmolliColormapBlue3TArray();

std::vector < unsigned short > TOMATOLIB_EXPORT getShmolliColormapRed15TArray();
std::vector < unsigned short > TOMATOLIB_EXPORT getShmolliColormapGreen15TArray();
std::vector < unsigned short > TOMATOLIB_EXPORT getShmolliColormapBlue15TArray();


#endif //USE_ITK

#endif //TomatoLIB_SHMOLLICOLORMAP_H
