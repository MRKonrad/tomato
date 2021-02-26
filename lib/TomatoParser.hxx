/*!
 * \file TomatoParser.hxx
 * \author Konrad Werys
 * \date 2018/08/19
 */

#ifndef Tomato_TomatoPARSER_HXX
#define Tomato_TomatoPARSER_HXX


namespace Ox {

    template<typename MeasureType>
    int
    TomatoParser<MeasureType>
    ::parse() {

        FILE *fh = fopen(_filePath.c_str(), "rb");

        yaml_parser_t parser;
        yaml_token_t token;

        /* Initialize parser */
        if (!yaml_parser_initialize(&parser))
            throw std::runtime_error("Failed to initialize parser!\n");
        if (fh == NULL) {
            throw std::runtime_error("\'" + _filePath + "\' - failed to open file!\n");
        }

        /* Set input file */
        yaml_parser_set_input_file(&parser, fh);

        bool flagKeyToken = false;
        //bool flagFlowSequenceStartToken = false;
        bool flagFlowEntryToken = false;
        bool flagBlockEntryToken = false;
        std::string lastKeyTokenValue;
        std::string lastFlowEntryTokenValue;
        std::string lastBlockEntryTokenValue;

        std::vector<std::string> temp;

        do {
            yaml_parser_scan(&parser, &token);
            switch (token.type) {
                /* Token types (read before actual token) */
                case YAML_KEY_TOKEN: {
                    flagKeyToken = true;
                    break;
                }
                case YAML_VALUE_TOKEN: {
                    break;
                }
                case YAML_FLOW_SEQUENCE_START_TOKEN: {
                    flagFlowEntryToken = true;
                    break;
                }
                case YAML_FLOW_SEQUENCE_END_TOKEN: {
                    flagFlowEntryToken = false;
                    if (_sequences.find(lastKeyTokenValue) != _sequences.end()) {
                        _sequences[lastKeyTokenValue] = temp;
                    }
                    temp.clear();
                    lastFlowEntryTokenValue = "";
                    break;
                }
                case YAML_FLOW_ENTRY_TOKEN: {
                    flagFlowEntryToken = true;
                    break;
                }
                case YAML_BLOCK_SEQUENCE_START_TOKEN: {
                    flagBlockEntryToken = true;
                    break;
                }
                case YAML_BLOCK_END_TOKEN: {
                    flagBlockEntryToken = false;
                    if ((_sequences.find(lastKeyTokenValue) != _sequences.end()) && (!temp.empty())) {
                        _sequences[lastKeyTokenValue] = temp;
                    }
                    temp.clear();
                    lastBlockEntryTokenValue = "";
                    break;
                }
                case YAML_BLOCK_ENTRY_TOKEN: {
                    flagBlockEntryToken = true;
                    break;
                }
                case YAML_SCALAR_TOKEN: {
                    std::string scalar((char *) token.data.scalar.value);

                    // store the token name in lastKeyTokenValue or in _scalars
                    if (flagKeyToken) {
                        // end flow sequence
                        if (!lastFlowEntryTokenValue.empty()){
                            flagFlowEntryToken = false;
                            if (_sequences.find(lastKeyTokenValue) != _sequences.end()) {
                                _sequences[lastKeyTokenValue] = temp;
                            }
                            temp.clear();
                            lastFlowEntryTokenValue = "";
                        }
                        // end block sequence
                        if (!lastBlockEntryTokenValue.empty()) {
                            flagBlockEntryToken = false;
                            if ((_sequences.find(lastKeyTokenValue) != _sequences.end()) && (!temp.empty())) {
                                _sequences[lastKeyTokenValue] = temp;
                            }
                            temp.clear();
                            lastBlockEntryTokenValue = "";
                        }
                        flagKeyToken = false;
                        lastKeyTokenValue = scalar;
                    } else {
                        if (_scalars.find(lastKeyTokenValue.c_str()) != _scalars.end()) {
                            _scalars[lastKeyTokenValue] = scalar;
                        }
                    }

                    // add flow entry
                    if (flagFlowEntryToken) {
                        flagFlowEntryToken = false;
                        lastFlowEntryTokenValue = scalar;
                        temp.push_back(scalar);
                    }

                    // add block entry
                    if (flagBlockEntryToken) {
                        flagBlockEntryToken = false;
                        lastBlockEntryTokenValue = scalar;
                        temp.push_back(scalar);
                    }
                    break;
                }
                default:
                    break;
            }
            if (token.type != YAML_STREAM_END_TOKEN) yaml_token_delete(&token);

        } while (token.type != YAML_STREAM_END_TOKEN);
        yaml_token_delete(&token);
        /* END new code */

        /* Cleanup */
        yaml_parser_delete(&parser);
        fclose(fh);

        return 0; // EXIT_SUCCESS
    }

    template<typename MeasureType>
    void
    TomatoParser<MeasureType>
    ::disp(){

        for (SequenceMapType::iterator it=_sequences.begin(); it!=_sequences.end(); ++it) {
            printVector<std::string>(it->second, it->first );
        }
        for (ScalarMapType::iterator it=_scalars.begin(); it!=_scalars.end(); ++it) {
            std::cout << it->first << " = " << it->second << '\n';
        }
    }

    template<typename MeasureType>
    template< typename TYPE >
    void
    TomatoParser<MeasureType>
    ::printVector(std::vector<TYPE> myVector, std::string myVectorName){
        std::cout << myVectorName << " =" << std::endl;
        for (int i = 0; i < myVector.size(); ++i) {
            //std::cout << std::setw(7) << myVector[i];
            std::cout << "  " << myVector[i] << std::endl;
        }
        //std::cout << std::endl;
    }

} // namespace Ox

#endif //Tomato_TomatoPARSER_H
