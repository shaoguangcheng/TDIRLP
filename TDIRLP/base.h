#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <list>
#include <limits.h>

using namespace std;

/**
 * In this file, I define some useful data structure and variables.
 */

#ifndef DEBUGMSG
#define DEBUGMSG(msg) cout << "line: " << __LINE__ << " function: " << __func__ << " file: " << __FILE__ << ", message: " << msg << endl
#endif

/**
 * @brief UPBOUND the upper bound of x and y
 */
#define UPBOUND INT_MAX

/**
 * @brief LOWBOUND the low bound of x and y
 */
#define LOWBOUND INT_MIN

/**
 * @brief The orientation enum define the orientation of the half plane
 */
enum orientation{
    LEFT=0,
    RIGHT
};

/**
 * @brief The halfPlane struct for save the half plane
 */
class vertex;
class edge;
class halfPlane{
public :
    /**
     * @brief  xCoef*x+y*yCoef+bias<=0
     */
    double xCoef;
    double yCoef;
    double bias;

    orientation direction;

    halfPlane();
    halfPlane(double xCoef, double yCoef, double bias);
    halfPlane(const halfPlane &hp);

    halfPlane& operator = (const halfPlane& hp);

    bool isIntersectWithEdge(const edge& e) const;
    bool isVertexOnHalfPlane(const vertex& v) const;
    bool isEdgeOnHalfPlane(const edge& e) const;
    vertex intersectPoint(const halfPlane& hp) const;

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

    vertex& operator = (const vertex& v);

public :
    double x;
    double y;
};

/**
 * @brief The edge class define the edge in polygon
 */
class edge{
public :
    edge();
    edge(const vertex &start, const vertex &end);
    edge(const vertex &start, const vertex &end, const halfPlane& line);
    edge(const edge &e);

    edge& operator = (const edge& e);

public :
    vertex start;
    vertex end;
    halfPlane line;

private :
    void getHalfPlane();
};

/**
 * @brief The polygon class define the polygon structure
 */
class polygon{
public :
    list<edge> edges;
    list<vertex> vertice;

    polygon();
    polygon(const list<edge>& edges);
    polygon(const polygon& p);

    polygon& operator = (const polygon& p);

    /**
     * @brief intersectOfHalfPlane using a half plane to cut a polygon and obtain a new polygon
     * @param hp
     * @return
     */
    polygon intersectOfHalfPlane(const halfPlane& hp);
};

ostream& operator << (ostream& out, const vertex& v);
ostream& operator << (ostream& out, const edge& e);
ostream& operator << (ostream& out, const polygon& p);

#endif // BASE_H
