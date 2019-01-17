/*!
 * \file TomatoParser.h
 * \author Konrad Werys
 * \date 2018/08/19
 */

#ifndef Tomato_TomatoPARSER_H
#define Tomato_TomatoPARSER_H

#include <map>
#include <string>
#include <vector>

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

#ifdef TomatoLib_EXPORTS
#include "TomatoParser.hxx"
#endif //TomatoLib_EXPORTS

#endif //Tomato_TomatoPARSER_H
