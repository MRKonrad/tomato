/*!
 * \file OxShmolli2Parser_test.cpp
 * \author Konrad Werys
 * \date 2018/08/19
 */

#include "gtest/gtest.h"

#include "OxShmolli2Parser.h"

TEST(OxShmolli2Parser, parse_yaml_file) {

    typedef double TYPE;
    char filePath [] = "testData/blood.yaml";

    Ox::OxShmolli2Parser<TYPE> parser;
    parser._filePath = filePath;
    parser._sequences["signalMag"]; // needed to 'add' sigMag
    parser.parse();
    //parser.disp();

    std::vector<std::string> truth;
    truth.push_back("55");
    truth.push_back("49");
    truth.push_back("29");
    truth.push_back("22");
    truth.push_back("51");
    truth.push_back("61");
    truth.push_back("64");

    EXPECT_EQ(parser._sequences["signalMag"], truth);
}

