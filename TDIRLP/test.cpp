#include "test.h"

void testHalfPlane()
{
    halfPlane hp(-1,1,1);

    if(LEFT == hp.direction)
        cout << "left" << endl;
    else
        cout << "right" << endl;

    vertex v(1,0);
    if(hp.isVertexOnHalfPlane(v))
        cout << "on" << endl;
    else
        cout << "not on" << endl;
}


void testPolygon()
{
    edge e1(vertex(0,0), vertex(0,1));
    edge e2(vertex(0,1), vertex(1,0));
    edge e3(vertex(1,0), vertex(0,0));

    list<edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);

    polygon p(edges);

    cout << e1 << endl;
    cout << p << endl;
}
