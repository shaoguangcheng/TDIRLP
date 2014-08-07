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

bool notGreatThan(double x, double y)
{
    return lessThan(x,y) || equal(x,y);
}

bool notLessThan(double x, double y)
{
    return greaterThan(x,y) || equal(x,y);
}
