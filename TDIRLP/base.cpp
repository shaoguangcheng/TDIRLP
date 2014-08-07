#include "base.h"
#include "util.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * the defination of half plane
 */
halfPlane::halfPlane()
{}

halfPlane::halfPlane(double xCoef, double yCoef, double bias)
{
    if(equal(0, xCoef)&&equal(0, yCoef)){
        DEBUGMSG("xCoef and yCoef can not be zero in the same time");
        return;
    }

    this->xCoef = xCoef;
    this->yCoef = yCoef;
    this->bias  = bias;

    getOrientation();
}

halfPlane::halfPlane(const halfPlane& hp) :
    xCoef(hp.xCoef),
    yCoef(hp.yCoef),
    bias(hp.bias),
    direction(hp.direction)
{}

halfPlane& halfPlane::operator = (const halfPlane& hp)
{
    xCoef = hp.xCoef;
    yCoef = hp.yCoef;
    bias  = hp.bias;
    direction = hp.direction;

    return *this;
}

void halfPlane::getOrientation()
{
    double x0 = -1.0*bias/xCoef, y0 = -1.0*bias/yCoef;

    /**
     * if yCoef*y <= 0
     */
    if(equal(0, bias)&&equal(0, xCoef)){
        if(lessThan(yCoef, 0)){
            direction = LEFT;
            goto end;
        }
        else{
            direction = RIGHT;
            goto end;
        }
    }

    /**
     * if yCoef*y+bias <= 0
     */
    if(equal(0, xCoef)){
        if(greaterThan(yCoef, 0)){
            direction = RIGHT;
            goto end;
        }
        else{
            direction = LEFT;
            goto end;
        }
    }

    /**
     * if xCoef*x <= 0
     */
    if(equal(0, bias)&&equal(0, yCoef)){
        if(lessThan(xCoef, 0)){
            direction = RIGHT;
            goto end;
        }
        else{
            direction = LEFT;
            goto end;
        }
    }

    /**
     * if xCoef*x+bias <= 0
     */
    if(equal(0, yCoef)){
        if(greaterThan(xCoef, 0)){
            direction = LEFT;
            goto end;
        }
        else{
            direction = RIGHT;
            goto end;
        }
    }

    /**
     * if xCoef*x+yCoef*y <= 0
     */
    if(equal(0, bias)){
        if(lessThan(xCoef, 0)){
            direction = RIGHT;
            goto end;
        }
        else{
            direction = LEFT;
            goto end;
        }
    }

    /**
     * if xCoef*x+yCoef*y+bias <= 0
     */
    if(greaterThan(x0, 0)&&greaterThan(y0, 0)){
        if(greaterThan(bias, 0)){
            direction = RIGHT;
            goto end;
        }
        else{
            direction = LEFT;
            goto end;
        }
    }

    if(lessThan(x0, 0)&&greaterThan(y0, 0)){
        if(greaterThan(bias, 0)){
            direction = LEFT;
            goto end;
        }
        else{
            direction = RIGHT;
            goto end;
        }
    }

    if(lessThan(x0, 0)&&lessThan(y0, 0)){
        if(greaterThan(bias, 0)){
            direction = LEFT;
            goto end;
        }
        else{
            direction = RIGHT;
            goto end;
        }
    }

    if(greaterThan(x0, 0)&&lessThan(y0, 0)){
        if(greaterThan(bias, 0)){
            direction = RIGHT;
            goto end;
        }
        else{
            direction = LEFT;
            goto end;
        }
    }

    end :
    return;
}

//////////////////////////////////////////////////////////////////////
/**
 * @brief vertex the defination of vertex
 */
vertex::vertex()
{}

vertex::vertex(double x, double y)
{
    this->x = x;
    this->y = y;
}

vertex::vertex(const vertex &v) :
    x(v.x),
    y(v.y)
{}

vertex& vertex::operator = (const vertex& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

/////////////////////////////////////////////////////////////////////
/**
 * @brief edge the defination of edge
 */
edge::edge()
{}

edge::edge(const vertex &start, const vertex &end)
{
    this->start = start;
    this->end   = end;

    getHalfPlane();
}

edge::edge(const vertex &start, const vertex &end, const halfPlane &line)
{
    this->start = start;
    this->end   = end;
    this->line  = line;
}

edge::edge(const edge &e) :
    start(e.start),
    end(e.end),
    line(e.line)
{}

edge& edge::operator = (const edge& e)
{
    start = e.start;
    end   = e.end;
    line  = e.line;

    return *this;
}

void edge::getHalfPlane()
{

}

orientation edge::getOrientation(const vertex &v) const
{}
