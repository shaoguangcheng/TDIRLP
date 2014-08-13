#include "util.h"

int exp(int x, int n)
{
    int tmp = 1;

    while(n--){
        tmp *= x;
    }

    return tmp;
}

double setPrecision(double x, int n)
{
    int tmp = exp(10,n);

    int y = int(x*tmp+0.5);

    return y*1.0/tmp;
}

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

//bool equal(const double &x, const double &y)
//{
//    return fabs(x-y) < EPS;
//}

bool notGreatThan(double x, double y)
{
    return lessThan(x,y) || equal(x,y);
}

bool notLessThan(double x, double y)
{
    return greaterThan(x,y) || equal(x,y);
}
