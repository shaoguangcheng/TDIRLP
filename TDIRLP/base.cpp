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
    if(equal(0.0, xCoef)&&equal(0.0, yCoef)){
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

bool halfPlane::isIntersectWithEdge(const edge& e) const
{
    return (
            isVertexOnHalfPlane(e.start)&&
            (!isVertexOnHalfPlane(e.end))
           ) ||
           (
            (!isVertexOnHalfPlane(e.start))&&
            (isVertexOnHalfPlane(e.end))
           ) ||
           (
            (isVertexOnBoundary(e.start))&&
            (isVertexOnBoundary(e.end))
           );
}

bool halfPlane::isVertexOnHalfPlane(const vertex &v) const
{
    return notGreatThan(xCoef*v.x+yCoef*v.y+bias, 0.0);
}

bool halfPlane::isVertexOnBoundary(const vertex& v) const
{
    return equal(xCoef*v.x+yCoef*v.y+bias, 0.0);
}

bool halfPlane::isEdgeOnHalfPlane(const edge& e) const
{
    return isVertexOnHalfPlane(e.start)&&
            isVertexOnHalfPlane(e.end);
}

vertex halfPlane::intersectPoint(const halfPlane& hp) const
{
    double tmp = xCoef*hp.yCoef - hp.xCoef*yCoef;

    if(equal(tmp, 0.0)){
        DEBUGMSG("two lines are parallel, they have no intersection");
        exit(EXIT_FAILURE);
    }

    vertex intersection;
    intersection.y = (xCoef*hp.bias - hp.xCoef*bias)*-1.0/tmp;
    intersection.x = (yCoef*hp.bias - hp.yCoef*bias)*1.0/tmp;

    return intersection;
}

void halfPlane::getOrientation()
{
    double x0 = -1.0*bias/xCoef, y0 = -1.0*bias/yCoef;

    /**
     * if yCoef*y <= 0
     */
    if(equal(0.0, bias)&&equal(0.0, xCoef)){
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
    if(equal(0.0, xCoef)){
        if(greaterThan(yCoef, 0.0)){
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
    if(equal(0.0, bias)&&equal(0.0, yCoef)){
        if(lessThan(xCoef, 0.0)){
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
    if(equal(0.0, yCoef)){
        if(greaterThan(xCoef, 0.0)){
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
    if(equal(0.0, bias)){
        if(lessThan(xCoef, 0.0)){
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
    if(greaterThan(x0, 0.0)&&greaterThan(y0, 0.0)){
        if(greaterThan(bias, 0.0)){
            direction = RIGHT;
            goto end;
        }
        else{
            direction = LEFT;
            goto end;
        }
    }

    if(lessThan(x0, 0.0)&&greaterThan(y0, 0.0)){
        if(greaterThan(bias, 0.0)){
            direction = LEFT;
            goto end;
        }
        else{
            direction = RIGHT;
            goto end;
        }
    }

    if(lessThan(x0, 0.0)&&lessThan(y0, 0.0)){
        if(greaterThan(bias, 0.0)){
            direction = LEFT;
            goto end;
        }
        else{
            direction = RIGHT;
            goto end;
        }
    }

    if(greaterThan(x0, 0.0)&&lessThan(y0, 0.0)){
        if(greaterThan(bias, 0.0)){
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

bool vertex::operator == (const vertex& v) const
{
    return equal(x, v.x)&&equal(y, v.y);
}

bool vertex::operator != (const vertex& v) const
{
    return (!equal(x, v.x))||(!equal(y, v.y));
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
   double xCoef = end.y - start.y;
   double yCoef = start.x - end.x;
   double bias  = end.x*start.y - start.x*end.y;

   halfPlane hp(xCoef, yCoef, bias);

   line = hp;
}

//////////////////////////////////////////////////////////////////////////
/**
 * @brief polygon the defination of polygon
 */
polygon::polygon()
{}

polygon::polygon(const list<edge> &edges)
    :edges(edges)
{
    list<edge>::const_iterator it = edges.begin();
    this->vertice.push_back(it->start);

    for(;it != edges.end();it++)
        this->vertice.push_back(it->end);
}

polygon::polygon(const polygon &p) :
    edges(p.edges),
    vertice(p.vertice)
{}

polygon& polygon::operator = (const polygon& p)
{
    edges = p.edges;
    vertice = p.vertice;

    return *this;
}

polygon polygon::intersectOfHalfPlane(const halfPlane& hp)
{
    vertexSet vs;
    return intersectOfHalfPlane(hp, vs);
}

polygon polygon::intersectOfHalfPlane(const halfPlane& hp, vertexSet& intersectVertex)
{
    list<edge>::iterator edgeIt;
    list<vertex>::iterator vertexIt;

    list<edge> _edges_(edges);

    edges.clear();
    vertice.clear();


    for(edgeIt = _edges_.begin();edgeIt != _edges_.end();edgeIt++){
        edge e(*edgeIt);
        /**
         * if e on half plane, then save e
         */
        if(hp.isEdgeOnHalfPlane(e)&&
                (!(hp.isVertexOnBoundary(e.start)&&
                   (hp.isVertexOnBoundary(e.end))))
                ){
            size_t size = vertice.size();
            if(size == size_t(0))
                vertice.push_back(e.start);

            vertice.push_back(e.end);
        }

        /**
         * if e has intersection with half plane, then add a new edge to polygon
         */
        if(hp.isIntersectWithEdge(e)){
            if(hp.isVertexOnBoundary(e.start)&&(hp.isVertexOnBoundary(e.end))){
                vertice.push_back(e.end);
                continue;
            }

            vertex intersection = hp.intersectPoint(e.line);
            intersectVertex.push_back(intersection);

            if(hp.isVertexOnHalfPlane(e.start)){

                /**
                 * if the direction of e is from on the half plane to not on the half plane
                 */
                 size_t size = vertice.size();
                 if(size == size_t(0)){
                     vertice.push_back(e.start);
                     if(e.start != intersection)
                        vertice.push_back(intersection);
                 }
                 else{
                     if(intersection != vertice.back())
                        vertice.push_back(intersection);
                 }
            }
            else{

                /**
                 * if the direction of e is from not on the half plane to on the half plane
                 */
                vertice.push_back(intersection);
                if(e.end != intersection)
                    vertice.push_back(e.end);
            }
        }
    }

    /**
     * if the intersection is one of the vertice of polygon, then it must repeat
     */
    if(2 == intersectVertex.size())
        if(intersectVertex[0] == intersectVertex[1]){
            intersectVertex.pop_back();
            vertice.pop_back();
        }

    if(vertice.back() != vertice.front())
        vertice.push_back(vertice.front());

    vertexIt = vertice.begin();
    vertex v(*vertexIt);
    vertexIt++;
    for(;vertexIt != vertice.end(); vertexIt++){
        edge e(v, *vertexIt);
        edges.push_back(e);
        v = *vertexIt;
    }

    return *this;
}

/////////////////////////////////////////////////////////////////////////////`
ostream& operator << (ostream& out, const vertex& v)
{
    out << "(" << v.x << "," << v.y << ")" << " ";
    return out;
}

ostream& operator << (ostream& out, const edge& e)
{
    out << "from " << e.start << "to " << e.end;
    return out;
}

ostream& operator << (ostream& out, const polygon& p)
{
    if(p.vertice.size() == 0){
        DEBUGMSG("The vertice number of the polygon is zero. It is not a polygon anymore");
    }

    if(p.vertice.size() == 1){
        DEBUGMSG("The vertice number of the polygon is one. It becomes a point");
    }

    if(p.vertice.size() == 2){
        DEBUGMSG("The vertice number of the polygon is two. It becomes a line");
    }

    list<vertex>::const_iterator it = p.vertice.begin();

    for(;it != p.vertice.end();it++)
        out << *it;

    return out;
}

ostream& operator << (ostream& out, const halfPlane& c)
{
    char* fmt = new char[100];

    if(!equal(c.xCoef, 0))
        sprintf(fmt, "%lfx", c.xCoef);

    if(greaterThan(c.yCoef, 0))
        sprintf(fmt, "%s+%lfy", fmt, c.yCoef);

    if(lessThan(c.yCoef, 0))
        sprintf(fmt, "%s-%lfy", fmt, -1*c.yCoef);

    sprintf(fmt, "%s<=%lf", fmt, -1*c.bias);
    out << fmt;

    delete [] fmt;

    return out;
}
////////////////////////////////////////////////////////////////////////////
