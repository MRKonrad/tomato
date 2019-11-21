//
// Created by Konrad Werys on 18.11.19.
//

#ifndef TOMATO_GDCMTOMATOREADTAGS_H
#define TOMATO_GDCMTOMATOREADTAGS_H

int gdcmTomatoReadTags(std::vector<std::pair<int, int> > tags, std::string filename, std::string &tagContent, bool verbose = false);

#endif //TOMATO_GDCMTOMATOREADTAGS_H
