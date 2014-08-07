#include "util.h"

inline bool greater(double x, double y)
{
    return x > y + EPS;
}

inline bool less(double x, double y)
{
    return x < y - EPS;
}

inline bool equal(double x, double y)
{
    return fabs(x-y) < EPS;
}
