/*!
 * \file OxImageCalculator.h
 * \author Konrad Werys
 * \date 2018/08/07
 */

#ifndef OXSHMOLLI2_OXIMAGECALCULATOR_H
#define OXSHMOLLI2_OXIMAGECALCULATOR_H

#include "CmakeConfigForOxShmolli2.h"
#ifndef USE_ONLY_CPP98
#include <thread>
#endif

#include "OxCalculatorT1.h"

namespace Ox {

    template<typename MeasureType>
    class ImageCalculator {
    public:

        ImageCalculator(){
            _nCols = 0;
            _nRows = 0;
            _nSamples = 0;
            _useThreads = true;
        };
        virtual ~ImageCalculator(){};

        int calculateThreaded(){

            if (!_useThreads) {
                int posStart = 0;
                int posStop = _nCols * _nRows;
                calculateOneThread(posStart, posStop);
            }
            else {
#ifndef CXX_STANDARD_98

//                // get the threads ready
//                std::vector<std::thread> threads;
//                unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
//
//                std::cout << "Number of threads: " << concurentThreadsSupported << std::endl;
//
//                // calc the range of pixels for each thread
//                std::vector<int> limits = bounds(concurentThreadsSupported, _nCols * _nRows);
//
//                // threaded loop
//                for (int i = 0; i < concurentThreadsSupported; ++i){
//                    threads.push_back(std::thread(&OxCalculatorOld<TYPE>::calcT1MapOneThread, this, limits[i], limits[i+1]));
//                }
//
//                // finish threads
//                for(auto &t : threads){
//                    t.join();
//                }
#else
                std::cout << "Threads not supported in OxShmolli2 with C++98" << std::endl;
#endif
            }
            return 0; // EXIT_SUCCESS
        }


        int calculateOneThread(int posStart, int posStop){
            return 0; // EXIT_SUCCESS
        };

    protected:
        bool _useThreads;
        int _nCols;
        int _nRows;
        int _nSamples;
        MeasureType** _imagesMag;
        MeasureType** _imagesPha;

        CalculatorT1<MeasureType> *calculatorT1;
    };

} // namespace Ox

#endif //OXSHMOLLI2_OXIMAGECALCULATOR_H
