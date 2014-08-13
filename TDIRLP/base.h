#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <list>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


/**
 * In this file, I define some useful data structure and variables.
 */

#ifndef DEBUGMSG
#define DEBUGMSG(msg) cout << "line: " << __LINE__ /*<< ", function: " << __func__ */<< ", file: " << __FILE__ << ", message: " << msg << endl
#endif

/**
 * @brief UPBOUND the upper bound of x and y
 */
#define UPBOUND (1e6*1.0)

/**
 * @brief LOWBOUND the low bound of x and y
 */
#define LOWBOUND (1e6*-1.0)

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
    bool isVertexOnBoundary(const vertex& v) const;
    bool isEdgeOnHalfPlane(const edge& e) const;
    vertex intersectPoint(const halfPlane& hp) const;

    string toString() const;

    void clear();

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

    bool operator == (const vertex& v) const;
    bool operator != (const vertex& v) const;

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

typedef vector<vertex> vertexSet;

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

    void clear();

    /**
     * @brief intersectOfHalfPlane using a half plane to cut a polygon and obtain a new polygon
     * @param hp
     * @return
     */
    polygon intersectOfHalfPlane(const halfPlane& hp);
    polygon intersectOfHalfPlane(const halfPlane& hp, vertexSet& intersectVertex);
};

ostream& operator << (ostream& out, const vertex& v);
ostream& operator << (ostream& out, const edge& e);
ostream& operator << (ostream& out, const polygon& p);
ostream& operator << (ostream& out, const halfPlane& c);

#endif // BASE_H
