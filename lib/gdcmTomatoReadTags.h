//
// Created by Konrad Werys on 18.11.19.
//

#ifndef TOMATO_GDCMTOMATOREADTAGS_H
#define TOMATO_GDCMTOMATOREADTAGS_H

int gdcmTomatoReadTags(const std::vector<std::pair<int, int> > tags, const std::string filename, std::string &tagContent, int verbose = 0);

#endif //TOMATO_GDCMTOMATOREADTAGS_H
