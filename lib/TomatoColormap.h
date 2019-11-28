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

#if defined __UINT32_MAX__ or UINT32_MAX
#include <cstdint>
#else
typedef unsigned short uint16_t;
#endif

std::string getEncodedSting(std::vector < uint16_t > encodeSource);

std::vector < uint16_t > getShmolliColormapRed3TArray();
std::vector < uint16_t > getShmolliColormapGreen3TArray();
std::vector < uint16_t > getShmolliColormapBlue3TArray();

std::vector < uint16_t > getShmolliColormapRed15TArray();
std::vector < uint16_t > getShmolliColormapGreen15TArray();
std::vector < uint16_t > getShmolliColormapBlue15TArray();


#endif //USE_ITK

#endif //TomatoLIB_SHMOLLICOLORMAP_H
