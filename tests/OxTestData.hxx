/*!
 * \file OxTestData.hxx
 * \author Konrad Werys
 * \date 2018/08/30
 */

#ifndef OXSHMOLLI2_OXTESTDATA_HXX
#define OXSHMOLLI2_OXTESTDATA_HXX

//#include "OxTestData.h"


namespace Ox {

    template< typename MeasureType >
    TestData<MeasureType>
    ::TestData(char* filePath){

        // based on https://www.wpsoftware.net/andrew/pages/libyaml.html

        FILE *fh = fopen(filePath, "rb");

        yaml_parser_t parser;
        yaml_token_t token;

        /* Initialize parser */
        if(!yaml_parser_initialize(&parser))
            fputs("Failed to initialize parser!\n", stderr);
        if(fh == NULL)
            fputs("Failed to open file!\n", stderr);

        /* Set input file */
        yaml_parser_set_input_file(&parser, fh);

        bool flagKeyToken = false;
        //bool flagFlowSequenceStartToken = false;
        bool flagFlowEntryToken = false;
        std::string lastKeyTokenValue;
        std::string lastFlowEntryTokenValue;

        std::vector<std::string> temp;

        do {
            yaml_parser_scan(&parser, &token);
            switch(token.type)
            {
                /* Token types (read before actual token) */
                case YAML_KEY_TOKEN:   {
                    flagKeyToken = true;
                    break;
                }
                case YAML_VALUE_TOKEN: break;
                case YAML_FLOW_SEQUENCE_START_TOKEN: {
                    //flagFlowSequenceStartToken = true;
                    flagFlowEntryToken = true;
                    //printf("\n(Flow sequence start) ");
                    break;
                }
                case YAML_FLOW_SEQUENCE_END_TOKEN: {

                    flagFlowEntryToken = false;

                    if (lastKeyTokenValue == "signalMag") copyStrVectorToMemberVector(temp, _signalMag);
                    if (lastKeyTokenValue == "signalPha") copyStrVectorToMemberVector(temp, _signalPha);
                    if (lastKeyTokenValue == "signs")     copyStrVectorToMemberVector(temp, _signs);
                    if (lastKeyTokenValue == "invTimes")  copyStrVectorToMemberVector(temp, _invTimes);

                    _nSamples = _invTimes.size();

                    temp.clear();

                    break;
                }
                case YAML_FLOW_ENTRY_TOKEN: {
                    flagFlowEntryToken = true;
                    break;
                }
                case YAML_SCALAR_TOKEN: {
                    std::string scalar((char*)token.data.scalar.value);
                    if (flagKeyToken){
                        flagKeyToken = false;
                        lastKeyTokenValue = scalar;
                    }

                    if (flagFlowEntryToken){
                        flagFlowEntryToken = false;
                        lastFlowEntryTokenValue = scalar;
                        temp.push_back(scalar);
                    }
                    break;
                }
                default:
                    break;
            }
            if(token.type != YAML_STREAM_END_TOKEN)
                yaml_token_delete(&token);
        } while(token.type != YAML_STREAM_END_TOKEN);
        yaml_token_delete(&token);
        /* END new code */

        /* Cleanup */
        yaml_parser_delete(&parser);
        fclose(fh);
    }

    template< typename MeasureType >
    template< typename TYPE >
    void
    TestData<MeasureType>
    ::printVector(std::vector<TYPE> myVector, std::string myVectorName){
        std::cout << myVectorName << " =";
        for (int i = 0; i < myVector.size(); ++i) {
            std::cout << std::setw(6) << myVector[i];
        }
        std::cout << std::endl;
    }

    template< typename MeasureType >
    void
    TestData<MeasureType>
    ::copyStrVectorToMemberVector(std::vector<std::string> strVector, std::vector<MeasureType> &memberVector) {

        //memberVector.resize(strVector.size());
        for (int i = 0; i < strVector.size(); ++i) {
            memberVector.push_back(KWUtil::StringToNumber<MeasureType>(strVector[i]));
//            memberVector[i] = KWUtil::StringToNumber<MeasureType>(strVector[i]);
        }
    }

    template< typename MeasureType >
    void
    TestData<MeasureType>
    ::disp(){
        std::cout << std::endl;
        printVector<MeasureType>(_signalMag, "signalMag ");
        printVector<MeasureType>(_signalPha, "signalPha ");
        printVector<MeasureType>(_signs,     "signs     ");
        printVector<MeasureType>(_invTimes,  "invTimes  ");
    };

} // namespace Ox



#endif //OXSHMOLLI2_OXTESTDATA_HXX
