//
// Created by Konrad Werys on 18.11.19.
//

#ifndef TOMATO_GDCMTOMATOREADTAGS_H
#define TOMATO_GDCMTOMATOREADTAGS_H

#include "CmakeConfigForTomato.h"

#ifdef USE_ITK
#include "tomatolib_export.h"

#include <string>
#include <vector>
#include <utility>

int TOMATOLIB_EXPORT gdcmTomatoReadTags(
        std::vector<std::pair<int, int> > tags,
        std::string filename,
        std::string &tagContent,
        bool verbose);

#endif //USE_ITK

#endif //TOMATO_GDCMTOMATOREADTAGS_H
