#ifndef UTIL_H
#define UTIL_H

#include <math.h>

#define EPS 1e-6

/**
 * @brief greater if x > y
 * @param x
 * @param y
 * @return
 */
inline bool greater(double x, double y);

/**
 * @brief less if x < y
 * @param x
 * @param y
 * @return
 */
inline bool less(double x, double y);

/**
 * @brief equal if x = y
 * @param x
 * @param y
 */
inline bool equal(double x, double y);

#endif // UTIL_H
