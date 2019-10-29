/*!
 * \file OxTestData.hxx
 * \author Konrad Werys
 * \date 2018/07/30
 */

#ifndef Tomato_OXTESTDATA_HXX
#define Tomato_OXTESTDATA_HXX


namespace Ox {

    template< typename MeasureType >
    TestData<MeasureType>
    ::TestData(char* filePath){

        Ox::TomatoParser<MeasureType> parser;
        parser._filePath = filePath;
        
        // tokens to be parsed
        parser._sequences["invTimes"];
        parser._sequences["signalMag"]; 
        parser._sequences["signalPha"]; 
        parser._sequences["signs"]; 

        parser._sequences["resultsMolli"]; 
        parser._sequences["resultsShmolli"];
        parser._sequences["resultsTwoParam"];
        parser._sequences["resultsThreeParam"];

        parser.parse();
        
        copyStrVectorToMemberVector(parser._sequences["signalMag"], _signalMag);
        copyStrVectorToMemberVector(parser._sequences["signalPha"], _signalPha);
        copyStrVectorToMemberVector(parser._sequences["signs"], _signs);
        copyStrVectorToMemberVector(parser._sequences["invTimes"], _invTimes);

        copyStrVectorToMemberVector(parser._sequences["resultsMolli"], _resultsMolli);
        copyStrVectorToMemberVector(parser._sequences["resultsShmolli"], _resultsShmolli);
        copyStrVectorToMemberVector(parser._sequences["resultsTwoParam"], _resultsTwoParam);
        copyStrVectorToMemberVector(parser._sequences["resultsThreeParam"], _resultsThreeParam);

        _nSamples = _invTimes.size();
        
        calcSignal();
    }

    template< typename MeasureType >
    void
    TestData<MeasureType>
    ::calcSignal(){
        _signal.resize(_nSamples);
        if (_signs.size() > 0) {
            for (int i = 0; i < _nSamples; ++i) {
                _signal[i] = _signalMag[i] * _signs[i];
            }
        } else {
            for (int i = 0; i < _nSamples; ++i){
                _signal[i] = _signalMag[i];
            }
        }
    }

    template< typename MeasureType >
    template< typename TYPE >
    void
    TestData<MeasureType>
    ::printVector(std::vector<TYPE> myVector, std::string myVectorName){
        std::cout << myVectorName << " =";
        for (int i = 0; i < myVector.size(); ++i) {
            std::cout << std::setw(7) << myVector[i];
        }
        std::cout << std::endl;
    }

    template< typename MeasureType >
    void
    TestData<MeasureType>
    ::copyStrVectorToMemberVector(std::vector<std::string> strVector, std::vector<MeasureType> &memberVector) {

        //memberVector.resize(strVector.size());
        for (unsigned int i = 0; i < strVector.size(); ++i) {
            memberVector.push_back(KWUtil::StringToNumber<MeasureType>(strVector[i]));
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

        printVector<MeasureType>(_resultsMolli,   "resultsMolli      ");
        printVector<MeasureType>(_resultsShmolli, "resultsShmolli    ");
        printVector<MeasureType>(_resultsMolli,   "resultsTwoParam   ");
        printVector<MeasureType>(_resultsShmolli, "resultsThreeParam ");
    };

} // namespace Ox



#endif //Tomato_OXTESTDATA_HXX
