/*!
 * \file OxShmolli2Parser.h
 * \author Konrad Werys
 * \date 2018/08/19
 */

#ifndef OXSHMOLLI2_OXSHMOLLI2PARSER_H
#define OXSHMOLLI2_OXSHMOLLI2PARSER_H

#include <map>
#include <string>
#include <vector>

#include "yaml.h"

#include "KWUtil.h"

namespace Ox {

    template<typename MeasureType>
    class OxShmolli2Parser {

    public:

        typedef std::map<std::string, std::string> ScalarMapType;
        typedef std::map<std::string, std::vector<std::string> > SequenceMapType;

        std::string _filePath;
        ScalarMapType _scalars;
        SequenceMapType _sequences;

        /**
         * based on https://www.wpsoftware.net/andrew/pages/libyaml.html
         * @return success/failure
         */
        int parse();

        void disp();

        template< typename TYPE >
        void printVector(std::vector<TYPE> myVector, std::string myVectorName);

    };
} // namespace Ox

#include "OxShmolli2Parser.hxx"

#endif //OXSHMOLLI2_OXSHMOLLI2PARSER_H
