#include "util.h"

bool greaterThan(double x, double y)
{
    return x > y + EPS;
}

bool lessThan(double x, double y)
{
    return x < y - EPS;
}

bool equal(double x, double y)
{
    return fabs(x-y) < EPS;
}
