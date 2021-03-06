#ifndef UTIL_H
#define UTIL_H

#include <math.h>

#define PRECISION 2

#define EPS 1e-6

/**
 * @brief exp calcualte x*x*x*....
 * @param x
 * @param n
 * @return
 */
int exp(int x, int n);

/**
 * @brief setPrecision reserve n digits for x
 * @param x
 * @param n
 * @return
 */
double setPrecision(double x, int n);

/**
 * @brief greaterThan if x > y
 * @param x
 * @param y
 * @return
 */
bool greaterThan(double x, double y);

/**
 * @brief lessThan if x < y
 * @param x
 * @param y
 * @return
 */
bool lessThan(double x, double y);

/**
 * @brief equal if x = y
 * @param x
 * @param y
 */
bool equal(double x, double y);
//bool equal(const double& x, const double& y);

/**
 * @brief notGreatThan  if x <= y
 * @param x
 * @param y
 * @return
 */
bool notGreatThan(double x, double y);

/**
 * @brief notLessThan if x >= y
 * @param x
 * @param y
 * @return
 */
bool notLessThan(double x, double y);


template <class T>
T findMax(const T* array, int N, int& index)
{
    T tmp = array[0];
    for(int i=1; i < N;i++){
        if(tmp < array[i]){
            tmp = array[i];
            index = i;
        }
    }

    return array[index];
}

#endif // UTIL_H
