#include "base.h"

/**
 * the defination of half plane
 */
halfPlane::halfPlane(double xCoef, double yCoef, double bias)
{
    this->xCoef = xCoef;
    this->yCoef = yCoef;
    this->bias  = bias;
}

void halfPlane::getOrientation()
{

}
