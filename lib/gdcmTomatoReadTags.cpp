//
// Created by Konrad Werys on 18.11.19.
//

#ifndef TOMATO_GDCMTOMATOREADTAGS_H
#define TOMATO_GDCMTOMATOREADTAGS_H

#include "gdcmTomatoReadTags.h"
#include "gdcmReader.h"
#include "gdcmDataElement.h"
#include "gdcmTag.h"

using namespace gdcm;

int gdcmTomatoReadTags(const std::vector<std::pair<int, int> > tags, const std::string filename, std::string &tagContent, bool verbose = false){

    tagContent = "";

    Reader reader;
    reader.SetFileName(filename.c_str());
    if(!reader.Read()){
        if (verbose) std::cerr << "readGDCMTags: could not read : " << filename << std::endl;
        return 1; // EXIT_FAILURE
    }

    File &file = reader.GetFile();
    DataSet &lastDataset = file.GetDataSet();

    for (size_t iTag = 0; iTag < tags.size(); iTag++){

        const Tag mytag(tags[iTag].first, tags[iTag].second);

        if (!lastDataset.FindDataElement(mytag)) {
            if (verbose) std::cerr << "readGDCMTags: Tag: "
                         " 0x" << std::hex << tags[iTag].first <<
                         " 0x" << std::hex << tags[iTag].second <<
                         " not found. " << std::endl;
            return 1; // EXIT_FAILURE
        }

        // it this is the last tag on the list, get it's value
        if (iTag == tags.size() - 1){

            const DataElement &mydataelement = lastDataset.GetDataElement(mytag);
            const ByteValue *myvalue = mydataelement.GetByteValue();
            if (!myvalue) {
                if (verbose) std::cerr << "readGDCMTags: Tag: "
                             " 0x" << std::hex << tags[iTag].first <<
                             " 0x" << std::hex << tags[iTag].second <<
                             " not read. " << std::endl;
                return 1; // EXIT_FAILURE
            }
            uint32_t mylength = uint32_t(myvalue->GetLength());
            char *mybuffer = new char[mylength + 1];
            myvalue->GetBuffer(mybuffer, mylength);
            tagContent = std::string(mybuffer, mylength);

            // clean up
            delete [] mybuffer;

            return 0; // EXIT_SUCCESS
        }

        const DataElement &seq = lastDataset.GetDataElement(mytag);

        SmartPointer<SequenceOfItems> sqi = seq.GetValueAsSQ();

        if (sqi->GetNumberOfItems() != 1) {
            if (verbose) std::cerr << "readGDCMTags: Tag: "
                         " 0x" << std::hex << tags[iTag].first <<
                         " 0x" << std::hex << tags[iTag].second <<
                         " wrong number of items, current version is working only with one item. " << std::endl;
            return 1; // EXIT_FAILURE
        }

        Item &item = sqi->GetItem(1);
        lastDataset = item.GetNestedDataSet();

    }

    return 0; // EXIT_SUCCESS
}

#endif //TOMATO_GDCMTOMATOREADTAGS_H
