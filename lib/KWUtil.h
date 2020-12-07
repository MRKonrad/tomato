//
//  KWUtil.h
//  TomatoLib
//
//  Created by Konrad Werys on 12/8/16.
//  Copyright © 2016 Konrad Werys. All rights reserved.
//

#ifndef KWUtil_h
#define KWUtil_h

//#ifndef EXIT_SUCCESS
//#define EXIT_SUCCESS 0
//#endif
//
//#ifndef EXIT_FAILURE
//#define EXIT_FAILURE 1
//#endif

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <fstream>
#include <limits>

// MS stuff problems
// https://stackoverflow.com/questions/26065359/m-pi-flagged-as-undeclared-identifier
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class KWUtil{
public:

    template< typename TYPE >
    static void printKW(bool doPrint, char* fmt, ...);

    /***********************/
    /* copyArrayToArray    */
    /***********************/
    template< typename TYPE1, typename TYPE2 >
    static void copyArrayToArray(int nSamples, TYPE1 *arrayTo, const TYPE2 *arrayFrom);

    /***********************/
    /* printVector         */
    /***********************/

    /** printVector
     * @tparam TYPE
     * @param name
     * @param vector
     */
    template<typename TYPE>
    static void printVector(const std::string& name, const std::vector<TYPE> vector);

    /** printVectorn
     * with newlines after every element
     * @tparam TYPE
     * @param name
     * @param vector
     */
    template<typename TYPE>
    static void printVectorNewline(const std::string &name, const std::vector<TYPE> vector);

    /***********************/
    /* printArray 1D       */
    /***********************/
    template< typename TYPE >
    static void printArray(int nSamples, const TYPE *myarray);

    template< typename TYPE >
    static void printArray(int nSamples, const TYPE *myarray, int width);

    template< typename TYPE >
    static void printArray(int nSamples, const TYPE *myarray, char* text);

    template< typename TYPE >
    static void printArray(bool doPrint, int nSamples, const TYPE *myarray);

    template< typename TYPE >
    static void printArray(bool doPrint, int nSamples, const TYPE *myarray, char* text);

    /***********************/
    /* printArray 2D       */
    /***********************/
    template< typename TYPE >
    static void printArray2D(int nRows, int nCols, TYPE **myarray);

    template< typename TYPE >
    static void printArray2D(int nRows, int nCols, TYPE **myarray, char* text);

    template< typename TYPE >
    static void printArray2D(bool doPrint, int nRows, int nCols, TYPE **myarray);

    template< typename TYPE >
    static void printArray2D(bool doPrint, int nRows, int nCols, TYPE **myarray, char* text);

    /***********************/
    /* printStdVector 1D   */
    /***********************/

    template< typename TYPE >
    static void printStdVector(const std::vector<TYPE> myvector);

    template< typename TYPE >
    static void printStdVector(const std::vector<TYPE> myvector, char* text);

    template< typename TYPE >
    static void printStdVector(bool doPrint, const std::vector<TYPE> myvector);

    template< typename TYPE >
    static void printStdVector(bool doPrint, const std::vector<TYPE> myvector, char* text);


    /***********************/
    /* calculations        */
    /***********************/

    template< typename TYPE >
    static void swap(TYPE &a, TYPE &b);

    template< typename TYPE >
    static TYPE max(TYPE a, TYPE b);

    template< typename TYPE >
    static TYPE min(TYPE a, TYPE b);

    template< typename TYPE >
    static TYPE calcSumArray(int nSamples, const TYPE *myarray);

    template< typename TYPE >
    static double calcMeanArray(int nSamples, const TYPE *myarray);

    template< typename TYPE >
    static double calcMedianArray(int nSamples, const TYPE *myarray);

    template< typename TYPE >
    static double calcStandardDeviationArray(int nSamples, const TYPE *myarray);

    template< typename TYPE >
    static double calcR2ss (int nSamples, const TYPE *fitted, const TYPE *ysignal);

    template< typename TYPE >
    static double calcR2cor(int nSamples, const TYPE *fitted, const TYPE *ysignal);

    template< typename TYPE >
    static int linearFit(
            int nSamples,
            const TYPE *datax,
            const TYPE *datay,
            TYPE &a,
            TYPE &b,
            TYPE &siga,
            TYPE &sigb,
            TYPE &R2,
            TYPE &chi2);

    template< typename TYPE >
    static double SKPLinReg(const TYPE *datax, const TYPE *datay, int nSamples, TYPE &rslope, TYPE &roffset);

    template< typename TYPE >
    static void SKPsort(int nSamples, const TYPE *myarray, int *index);

    template< typename TYPE >
    static void quickSort(int nSamples, TYPE *myarray);

    template< typename TYPE >
    static void quickSortIndex(int nSamples, TYPE *myarray, int *indexArray);

    template< typename TYPE >
    static TYPE getChiSqrt(TYPE lastFValue, int nSamples);

    template< typename TYPE >
    static TYPE calcFeFromT2(TYPE T2);

    template< typename TYPE >
    static TYPE calcFeErrorFromT2(TYPE T2, TYPE deltaT2);

    template< typename TYPE >
    static int calculateFitError(int nSamples, int nDims, const TYPE* jacobian, TYPE mFuncNorm, TYPE* fitError);

    /**
     *
     * @tparam TYPE
     * @param matrix - pointer to array with indices [0 1; 2 3]
     * @param matrixInverse - pointer to with indices [0 1; 2 3]. Fills the inverse matix if
     * determinant != 0 or zero matrix if det == 0
     * @return 0 if success, 1 if det == 0
     */
    template< typename TYPE >
    static int calcMatrixInverse2x2(const TYPE *matrix, TYPE *matrixInverse);

    /**
     *
     * @tparam TYPE
     * @param matrix - pointer to array with indices [0 1 2; 3 4 5; 6 7 8]
     * @param matrixInverse - pointer to with indices [0 1 2; 3 4 5; 6 7 8]. Fills the inverse matix if
     * determinant != 0 or zero matrix if det == 0
     * @return 0 if success, 1 if det == 0
     */
    template< typename TYPE >
    static int calcMatrixInverse3x3(const TYPE *matrix, TYPE *matrixInverse);

    template< typename TYPE >
    static TYPE MOLLI_min(TYPE a[], int n, int *indm);

    template< typename TYPE >
    static TYPE MOLLI_max(TYPE a[], int n, int *indm);

    /***********************/
    /* string operations   */
    /***********************/
    // http://www.cplusplus.com/articles/D9j2Nwbp/
    template < typename TYPE >
    static std::string NumberToString ( TYPE Number ){
         std::ostringstream ss;
         ss << Number;
         return ss.str();
    };

    //http://www.cplusplus.com/articles/D9j2Nwbp/
    template < typename TYPE >
    static TYPE StringToNumber ( const std::string &Text ){
        std::istringstream ss(Text);
        TYPE result;
        return ss >> result ? result : 0;
    };

    // for example to turn acquisition time to seconds
    template < typename TYPE >
    static TYPE dicomTime2Seconds(std::string dicomTimeString);

    /***********************/
    /* threading           */
    /***********************/
    template < typename TYPE >
    static std::vector<int> bounds(int parts, int mem);

    /***********************/
    /* files               */
    /***********************/

    static std::string PathSeparator(){
#if defined(WIN32) || defined(_WIN32) || defined __CYGWIN__
        return std::string("\\");
#else
        return std::string("/");
#endif
    }

    // http://www.cplusplus.com/reference/string/string/find_last_of/
    static void splitFilename (const std::string& str, std::string &path, std::string &file) {
        //std::cout << "Splitting: " << str << '\n';
        std::size_t found = str.find_last_of("/\\");
        path = str.substr(0,found);
        file = str.substr(found+1);
    }

    // http://www.cplusplus.com/doc/tutorial/files/
    static std::vector <std::string> readFile(const std::string filePath){
        std::vector <std::string> fileContent;
        std::string line;

        try {
            std::ifstream myfile(filePath.c_str());
            if (myfile.is_open()) {
                while (getline(myfile, line)) {
                    fileContent.push_back(line);
                }
                myfile.close();
            } else {
                printf("Unable to open file: %s", filePath.c_str() );
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            printf("Unable to read the file: %s", filePath.c_str() );
        }
        return fileContent;
    }

    /***********************/
    /* testing             */
    /***********************/
    template< typename TYPE >
    static bool array_expect_near(
            int nElements,
            const TYPE *array1,
            const TYPE *array2,
            TYPE abs_error = 0,
            const std::string& comment = "");

    //static bool isFile(const char* path) {
    //    struct stat buf;
    //    stat(path, &buf);
    //    return S_ISREG(buf.st_mode);
    //}

    //static bool isDir(const char* path) {
    //    struct stat buf;
    //    stat(path, &buf);
    //    return S_ISDIR(buf.st_mode);
    //}

private:
    // can this implementation of quicksort be improved? http://codereview.stackexchange.com/questions/77782/quick-sort-implementation
    template< typename TYPE >
    static void quickSort(TYPE arr[], int left, int right);

    template< typename TYPE >
    static void quickSortIndex(TYPE arr[], int indexArray[], int left, int right);

};

#include "KWUtil.hxx"

#endif /* KWUtil_h */
