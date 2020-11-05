/*!
 * \file TomatoParser.h
 * \author Konrad Werys
 * \date 2018/08/19
 */

#ifndef Tomato_TomatoPARSER_H
#define Tomato_TomatoPARSER_H

#include "CmakeConfigForTomato.h"
#ifdef USE_YAML

#include <map>
#include <string>
#include <vector>

#include "tomatolib_export.h"

#include "yaml.h"

#include "KWUtil.h"

namespace Ox {

    template<typename MeasureType>
    class TomatoParser {

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

#ifndef TOMATOLIB_COMPILED
#include "TomatoParser.hxx"
#endif //TOMATOLIB_COMPILED

#endif //USE_YAML

#endif //Tomato_TomatoPARSER_H
