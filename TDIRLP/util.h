#ifndef UTIL_H
#define UTIL_H

#include <math.h>

#define EPS 1e-6

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

#endif // UTIL_H
