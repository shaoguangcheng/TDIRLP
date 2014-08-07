#ifndef BASE_H
#define BASE_H

#include <ostream>
#include <list>

using namespace std;

/**
 * In this file, I define some useful data structure and variables.
 */

/**
 * @brief The orientation enum define the orientation of the half plane
 */
enum orientation{
    left=0,
    right
};

/**
 * @brief The halfPlane struct for save the half plane
 */
class halfPlane{
public :
    /**
     * @brief  xCoef*x+y*yCoef+bias<=0
     */
    double xCoef;
    double yCoef;
    double bias;

    orientation direction;

    halfPlane(double xCoef, double yCoef, double bias);
    halfPlane(const halfPlane* hp);

private :
    void getOrientation();
};

/**
 * @brief The vertex class define the vertex
 */
class vertex{
public :
    vertex();
    vertex(double x, double y);
    vertex(const vertex& v);

public :
    double x;
    double y;
};

class edge{
public :
    edge();
    edge(const vertex &start, const vertex &end);
    edge(const edge &e);

    orientation getOrientation(const vertex& v) const;

public :
    vertex start;
    vertex end;
    halfPlane line;

private :
    halfPlane getLine(const vertex &start, const vertex &end) const;
};

class polygon{
public :
    list<edge> edges;
    list<vertex> vertice;

    polygon(const list<edge>& edges);
    polygon(const polygon& p);
};

ostream& operator << (ostream& out, const vertex& v);
ostream& operator << (ostream& out, const polygon& p);

#endif // BASE_H
