#include "test.h"

void testHalfPlane()
{
    halfPlane hp(1,-1,0);

    if(LEFT == hp.direction)
        cout << "left" << endl;
    else
        cout << "right" << endl;

    vertex v(1,0);
    if(hp.isVertexOnHalfPlane(v))
        cout << "on" << endl;
    else
        cout << "not on" << endl;

    halfPlane hp2(1,1,1);
    cout << hp2.intersectPoint(hp) << endl;

    cout << 10*UPBOUND << " " << LOWBOUND << endl;
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
    cout << p.edges.size() << endl;
    halfPlane hp(0,-1,0.5);
    p.intersectOfHalfPlane(hp);

    cout << p.edges.size() << endl;
    cout << p << endl;
}

void testTDLP()
{
    TDLP tdlp;

    constraint c1(-1,0,0);
    constraint c2(0,-1,0);
    constraint c3(1,1,-1);
//    constraint c4(-1,0,0);
//    constraint c5(0,-1,0);
    objFunc fun(1,1);

    tdlp.addConstraint(c1);
    tdlp.addConstraint(c2);
    tdlp.addConstraint(c3);
//    tdlp.addConstraint(c4);
//    tdlp.addConstraint(c5);
    tdlp.setObjFunc(fun);

    solution s = tdlp.solve();

    cout << "=====================" << endl;
    cout << tdlp << endl;
    cout << "=====================" << endl;
    cout << s << endl;
}

void testFindMax()
{
    double v[4] = {0,2000,0,-2000};
    int index = 0;

    cout << findMax(v,4,index) << endl;
    cout << index << endl;
}
