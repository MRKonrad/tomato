//
//  KWUtil.hxx
//  OxShmolliLib
//
//  Created by Konrad Werys on 12/8/16.
//  Copyright © 2016 Konrad Werys. All rights reserved.
//

#ifndef KWUtil_hxx
#define KWUtil_hxx

//#include "KWUtil.h"

/***********************/
/* copyArrayToArray    */
/***********************/

template< typename TYPE1, typename TYPE2 >
void KWUtil::copyArrayToArray(int nSamples, TYPE1 *arrayTo, const TYPE2 *arrayFrom){

    if (arrayFrom == NULL){
        arrayTo = NULL;
        return;
    }

    for (int i = 0; i < nSamples; i++)
        arrayTo[i] = (TYPE1)arrayFrom[i];
}

/***********************/
/* printVector         */
/***********************/

template<typename TYPE>
void KWUtil::printVector(const std::string name, const std::vector<TYPE> vector){
    std::cout << name << std::endl;
    for (unsigned int i = 0; i < vector.size(); ++i){
        std::cout << "  " << vector.at(i) << std::endl;
    }
}

/***********************/
/* printArray 1D       */
/***********************/

template< typename TYPE >
void KWUtil::printArray(int nSamples, const TYPE *myarray){
    for (int i=0; i<nSamples; i++){
        std::cout << myarray[i] << " ";
    }
}

template< typename TYPE >
void KWUtil::printArray(int nSamples, const TYPE *myarray, int width){
    for (int i=0; i<nSamples; i++){
        std::cout << std::setw(width) << myarray[i] << " ";
    }
}

template< typename TYPE >
void KWUtil::printArray(int nSamples, const TYPE *myarray, char* text){
    std::cout << text;
    printArray(nSamples, myarray);
}

template< typename TYPE >
void KWUtil::printArray(bool doPrint, int nSamples, const TYPE *myarray){
    if(doPrint) printArray(nSamples, myarray);
}

template< typename TYPE >
void KWUtil::printArray(bool doPrint, int nSamples, const TYPE *myarray, char* text){
    if(doPrint) printArray(nSamples, myarray, text);
}

/***********************/
/* printArray 2D       */
/***********************/

template< typename TYPE >
void KWUtil::printArray2D(int nRows, int nCols, TYPE **myarray){
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++)
            std::cout << myarray[i][j] << "\t";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template< typename TYPE >
void KWUtil::printArray2D(int nRows, int nCols, TYPE **myarray, char * text){
    std::cout << text;
    printArray2D(nRows, nCols, myarray);
}

template< typename TYPE >
void KWUtil::printArray2D(bool doPrint, int nRows, int nCols, TYPE **myarray){
    if(doPrint) printArray2D(nRows, nCols, myarray);
}

template< typename TYPE >
void KWUtil::printArray2D(bool doPrint, int nRows, int nCols, TYPE **myarray, char * text){
    if(doPrint) printArray2D(nRows, nCols, myarray, text);
}


template< typename TYPE >
void KWUtil::printStdVector(const std::vector<TYPE> myvector){
    int nSamples = myvector.size();
    KWUtil::printArray(nSamples, &myvector[0]);
}

template< typename TYPE >
void KWUtil::printStdVector(const std::vector<TYPE> myvector, char* text){
    int nSamples = myvector.size();
    KWUtil::printArray(nSamples, &myvector[0], text);
}

template< typename TYPE >
void KWUtil::printStdVector(bool doPrint, const std::vector<TYPE> myvector){
    int nSamples = myvector.size();
    KWUtil::printArray(doPrint, nSamples, &myvector[0]);
}

template< typename TYPE >
void KWUtil::printStdVector(bool doPrint, const std::vector<TYPE> myvector, char* text){
    int nSamples = myvector.size();
    KWUtil::printArray(doPrint, nSamples, &myvector[0], text);
}

/***********************/
/* calculations        */
/***********************/

template< typename TYPE >
void KWUtil::swap(TYPE &a, TYPE &b){
    TYPE temp = a;
    a = b;
    b = temp;
}

template< typename TYPE >
TYPE KWUtil::max(TYPE a, TYPE b){
    return (a > b) ? a : b;
}

template< typename TYPE >
TYPE KWUtil::min(TYPE a, TYPE b){
    return (a < b) ? a : b;
}

template< typename TYPE >
TYPE KWUtil::calcSumArray(int nSamples, const TYPE *myarray){
    TYPE sum = TYPE(); // 0
    for(long i = 0; i < nSamples; i++){
        sum+= myarray[i];
    }
    return sum;
}

template< typename TYPE >
double KWUtil::calcMeanArray(int nSamples, const TYPE *myarray){
    if (nSamples == 0)
        return 0.0;
    else if (nSamples == 1)
        return myarray[0];
    else
        return (calcSumArray(nSamples,myarray)/nSamples);
}

template< typename TYPE >
double KWUtil::calcMedianArray(int nSamples, const TYPE *myarray){
    if (nSamples == 0){
        return 0.0;
    } else if (nSamples == 1){
        return myarray[0];
    } else {
        TYPE *sortedArray = new TYPE[nSamples];
        KWUtil::copyArrayToArray(nSamples, sortedArray, myarray);
        KWUtil::quickSort(nSamples, sortedArray);
		double result;
        if(nSamples % 2) {
            // if odd number of samples, return the middle
			result = sortedArray[(nSamples - 1) / 2];
        } else {
            // if even number of samples, return mean of the two in the middle
            int pos = nSamples/2-1;
            result = (sortedArray[pos] + sortedArray[pos+1]) / 2;
        }
        delete[] sortedArray;
        return result;
    }
}

template< typename TYPE >
double KWUtil::calcStandardDeviationArray(int nSamples, const TYPE *myarray) {
    if (nSamples < 2) {
        return 0.0;
    } else {
        TYPE mymean = KWUtil::calcMeanArray(nSamples, myarray);
        TYPE sum = 0.0;
        for (unsigned int i = 0; i < nSamples; ++i){
            sum = sum + (myarray[i] - mymean) * (myarray[i] - mymean);
        }
        return (sqrt(sum)/(nSamples-1));
    }
}

template< typename TYPE >
// calculate R square confidence map
double KWUtil::calcR2ss(int nSamples, const TYPE *fitted, const TYPE *ysignal){
    // equation taken from wikipedia https://en.wikipedia.org/wiki/Coefficient_of_determination
    TYPE meanYsignal = TYPE(); // 0
    TYPE SStot = TYPE(); // 0
    TYPE SSres = TYPE(); // 0

    for (int i = 0; i < nSamples; i++){
        meanYsignal = meanYsignal + ysignal[i];
    }
    meanYsignal = meanYsignal/nSamples;
    for (int i = 0; i < nSamples; i++){
        SStot = SStot + (ysignal[i] - meanYsignal)*(ysignal[i] - meanYsignal);
    }
    for (int i = 0; i < nSamples; i++){
        SSres = SSres + (ysignal[i] - fitted[i])*(ysignal[i] - fitted[i]);
    }
    if (SStot == 0)
        return 0.0;
    else
        return 1-SSres/SStot;
}

// calculate R square confidence map
template< typename TYPE >
double KWUtil::calcR2cor(int nSamples, const TYPE *fitted, const TYPE *ysignal){
    // calculated from squared pearson correlation coefficient
    // R2 = (covYF / (stdY*stdF))^2 = covYF^2 / (std2Y * std2F)
    // covYF = sum((y-meany)(calcCostValue-meanf))/N
    // stdY = sqrt(sum((y-meany)/N)
    // std2Y = sum((y-meany)/N
    // Ns are reduced, so not used in this calculations

    TYPE meanY = 0, meanF = 0, sumYY = 0, sumFF = 0, covYF = 0; //xf =sum((y-meany)(calcCostValue-meanf))
    for (int i = 0; i < nSamples; i++){
        meanY = meanY + ysignal[i];
        meanF = meanF + fitted[i];
    }
    meanY = meanY/nSamples;
    meanF = meanF/nSamples;
    for (int i = 0; i < nSamples; i++){
        sumYY = sumYY + (ysignal[i] - meanY)*(ysignal[i] - meanY);
        sumFF = sumFF + (fitted[i]  - meanF)*(fitted[i]  - meanF);
        covYF = covYF + (ysignal[i] - meanY)*(fitted[i]  - meanF);
    }
    if ((sumYY == 0) || (sumFF == 0))
        return 0.0;
    else
        return covYF*covYF/(sumYY*sumFF);
}

template< typename TYPE >
double KWUtil::SKPLinReg(const TYPE *datax, const TYPE *datay, int nSamples, TYPE &rslope, TYPE &roffset){
    // sets slope and offset and returns pearson's R between two vectors [0:n-1] elements each.
    TYPE yt, xt, syy = 0.0, sxy = 0.0, sxx = 0.0, ay = 0.0, ax = 0.0;
    if(nSamples<2) return(0.0); // sanity check
    for (long j = 0; j < nSamples; j++) // caclualte means
    {
        ax += datax[j];
        ay += datay[j];
    }
    ax /= nSamples;
    ay /= nSamples;
    for (long j = 0; j < nSamples; j++)
    {
        xt = datax[j]-ax;
        yt = datay[j]-ay;
        sxx += xt*xt;
        syy += yt*yt;
        sxy += xt*yt;
    }
    // error return in case no adequate diviation
    if (sxx*syy <=0)
    {
        rslope = 0.0;
        roffset = ay;
        return(0);
    }
    // else normal return
    rslope = sxy/sxx;
    roffset = ay-rslope*ax;
    return (sxy/sqrt(sxx*syy));   // correlation = (sxy/sqrt(sxx*syy)
} //double SKPLinReg(double *x,double *y, int n, double &rslope, double &roffset)

template< typename TYPE >
void KWUtil::SKPsort(int nSamples, const TYPE *myarray, int *index){
    // sorts array [1..nSamples+1] :/
    int l,j,ir,indxt,i;
    double q;
    for (j=1;j<=nSamples;j++) index[j]=j;
    if (nSamples == 1) return;
    l=(nSamples >> 1) + 1;
    ir=nSamples;
    for (;;)
    {
        if (l > 1)
            q=myarray[(indxt=index[--l])];
        else {
            q=myarray[(indxt=index[ir])];
            index[ir]=index[1];
            if (--ir == 1) {
                index[1]=indxt;
                return;
            }
        }
        i=l;
        j=l << 1;
        while (j <= ir) {
            if (j < ir && myarray[index[j]] < myarray[index[j+1]]) j++;
            if (q < myarray[index[j]]) {
                index[i]=index[j];
                j += (i=j);
            }
            else j=ir+1;
        }
        index[i]=indxt;
    }
}

template< typename TYPE >
void KWUtil::quickSort(int nSamples, TYPE *myarray){
    KWUtil::quickSort(myarray, 0, nSamples-1);
}

template< typename TYPE >
void KWUtil::quickSortIndex(int nSamples, TYPE *myarray, int *indexArray){
    for (int i = 0; i < nSamples; i++){
        indexArray[i] = i;
    }
    KWUtil::quickSortIndex(myarray, indexArray, 0, nSamples-1);
}

// private
template< typename TYPE >
void KWUtil::quickSort(TYPE arr[], int left, int right) {
    // http://www.algolist.net/Algorithms/Sorting/Quicksort
    int i = left, j = right;
    TYPE tmp;
    TYPE pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

template< typename TYPE >
void KWUtil::quickSortIndex(TYPE arr[], int indexArr[], int left, int right) {
    // http://www.algolist.net/Algorithms/Sorting/Quicksort
    int i = left, j = right, tmpIdx;
    TYPE tmp;
    TYPE pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            // array
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            // indexArray
            tmpIdx = indexArr[i];
            indexArr[i] = indexArr[j];
            indexArr[j] = tmpIdx;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        quickSortIndex(arr, indexArr, left, j);
    if (i < right)
        quickSortIndex(arr, indexArr, i, right);
}

template< typename TYPE >
void KWUtil::printKW(bool doPrint, char* fmt, ...){
    if (doPrint){
        va_list args;
        va_start(args,fmt);
        vprintf(fmt,args);
        va_end(args);
    }
}

template< typename TYPE >
int KWUtil::calcMatrixInverse3x3(const TYPE *matrix, TYPE *matrixInverse){

    for (int i = 0; i < 9; ++i){
        matrixInverse[i] = 0;
    }

    const TYPE *a = matrix; // just for shorter equations
    TYPE det = a[0]*a[4]*a[8] - a[0]*a[5]*a[7] - a[1]*a[3]*a[8] + a[1]*a[5]*a[6] + a[2]*a[3]*a[7] - a[2]*a[4]*a[6];

    if (fabs(det) < KWUtil::SMALLNUMBER){
        return 1; // EXIT_FAILURE
    }

    matrixInverse[0] =  (a[4]*a[8] - a[5]*a[7])/det;
    matrixInverse[1] = -(a[1]*a[8] - a[2]*a[7])/det;
    matrixInverse[2] =  (a[1]*a[5] - a[2]*a[4])/det;
    matrixInverse[3] = -(a[3]*a[8] - a[5]*a[6])/det;
    matrixInverse[4] =  (a[0]*a[8] - a[2]*a[6])/det;
    matrixInverse[5] = -(a[0]*a[5] - a[2]*a[3])/det;
    matrixInverse[6] =  (a[3]*a[7] - a[4]*a[6])/det;
    matrixInverse[7] = -(a[0]*a[7] - a[1]*a[6])/det;
    matrixInverse[8] =  (a[0]*a[4] - a[1]*a[3])/det;

    return 0; // EXIT_SUCCESS
}

template< typename TYPE >
TYPE KWUtil::getChiSqrt(TYPE lastFValue, int nSamples){
    return sqrt(lastFValue/(nSamples-1));
}

template< typename TYPE >
TYPE KWUtil::MOLLI_min(TYPE a[], int n, int *indm)
{
    int i=0;
    short mval=a[0];
    *indm=0;
    for (i=0;i<n;i++)if(mval>a[i]){*indm=i;mval=a[i];};
    return(mval);
}

template< typename TYPE >
TYPE KWUtil::MOLLI_max(TYPE a[], int n, int *indm)
{
    int i=0;
    short mval=a[0];
    *indm=0;
    for (i=0;i<n;i++)if(mval<a[i]){*indm=i;mval=a[i];};
    return(mval);
}

template< typename TYPE >
TYPE KWUtil::dicomTime2Seconds(std::string dicomTimeString){
    TYPE dicomTimeSeconds = 0;

    if (dicomTimeString.length() < 1) {
        // printf("ERROR: Dicom string should not be empty");
        return dicomTimeSeconds; // return 0
    }

    if (dicomTimeString.length() != 6 && dicomTimeString[6] != '.'){
        printf("ERROR: Dicom string should have a dot at the sixth index");
        return dicomTimeSeconds; // return 0
    }

    std::string hours = dicomTimeString.substr(0,2);
    std::string minutes = dicomTimeString.substr(2,2);
    std::string seconds = dicomTimeString.substr(4,2);
    std::string milliseconds = "0";
    if (dicomTimeString.length() > 6) {
        milliseconds = dicomTimeString.substr(6, dicomTimeString.length()-6);
    }

    TYPE hoursInSeconds = 60 * 60 * KWUtil::StringToNumber<TYPE>(hours);
    TYPE minutesInSeconds = 60 * KWUtil::StringToNumber<TYPE>(minutes);
    TYPE secondsInSeconds = 1 *KWUtil::StringToNumber<TYPE>(seconds);
    TYPE millisecondsInSeconds = KWUtil::StringToNumber<TYPE>(milliseconds);

    dicomTimeSeconds = hoursInSeconds + minutesInSeconds + secondsInSeconds + millisecondsInSeconds;

    return dicomTimeSeconds;
}

// for threading
// from https://solarianprogrammer.com/2012/02/27/cpp-11-thread-tutorial-part-2/
template < typename TYPE >
std::vector<int> KWUtil::bounds(int parts, int mem) {
    std::vector<int>bnd;
    int delta = mem / parts;
    int reminder = mem % parts;
    int N1 = 0, N2 = 0;
    bnd.push_back(N1);
    for (int i = 0; i < parts; ++i) {
        N2 = N1 + delta;
        if (i == parts - 1)
            N2 += reminder;
        bnd.push_back(N2);
        N1 = N2;
    }
    return bnd;
}

#endif
