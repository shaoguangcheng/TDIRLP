

#include <stdio.h>
#include <algorithm>
#include <string.h>

#include "TDLP.h"

TDLP::TDLP()
{
}

TDLP::TDLP(const TDLP &tdlp) :
    constraints(tdlp.getConstraint()),
    func(tdlp.getObjFunc())
{
}

TDLP::~TDLP()
{
}

TDLP& TDLP::operator = (const TDLP& tdlp)
{
    constraints = tdlp.getConstraint();
    func = tdlp.getObjFunc();

    return *this;
}

void TDLP::addConstraint(const constraint& c)
{
    constraints.push_back(c);
}

void TDLP::delConstraint(const constraint& c)
{
    constraintIterator it = find_if(constraints.begin(), constraints.end(),
                                    bind2nd(isConstraintIndentical(), c));
    if(it != constraints.end()){
        constraints.erase(it);
    }
}

void TDLP::setObjFunc(const objFunc &f)
{
    func = f;
}

constraintSet  TDLP::getConstraint() const
{
    return constraints;
}

objFunc TDLP::getObjFunc() const
{
    return func;
}

void TDLP::clear()
{
    constraints.clear();
    func.xCoef = 0;
    func.yCoef = 0;
}

solution TDLP::solve()
{
    ////////////////////////////////////////////////////////////////////
    /**
     * the initial optimal point
     */
    vertex v1(LOWBOUND, UPBOUND);
    vertex v2(UPBOUND, UPBOUND);
    vertex v3(UPBOUND, LOWBOUND);
    vertex v4(LOWBOUND, LOWBOUND);
    vertex v[4] = {v1, v2, v3, v4};

    double value[4];
    for(int i=0;i<4;i++){
        value[i] = func.getValue(v[i]);
    }

    int index = 0;
    findMax(value, 4, index);

    /**
     * @brief ans the optimal solution of the LP
     */
    solution ans(v[index], value[index]);

    /**
     * define the boundary plane
     */
    edge e1(v1, v2);
    edge e2(v2, v3);
    edge e3(v3, v4);
    edge e4(v4, v1);

    list<edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    edges.push_back(e4);

    region feasibleRegion(edges);
    /////////////////////////////////////////////////////////////////////

    /**
     * shuffle the constraints
     */
    random_shuffle(constraints.begin(), constraints.end());

    constConstraintIterator it;
    for(it = constraints.begin(); it != constraints.end(); it++){
        constraint c = *it;

        if(ans.getStatus() == singleLine){
            edge e(ans.getEdge());
            if(c.isEdgeOnHalfPlane(e)){
                feasibleRegion.intersectOfHalfPlane(c);
            }
            else{
                updateOptimalSolution(feasibleRegion, c, ans);
            }

            continue;
        }

        if(ans.getStatus() == singlePoint){
            if(c.isVertexOnHalfPlane(ans.getPoint())){

                /**
                 * if current optimal vertex is on the half plane that is introduced newly
                 */

                /**
                 * if current feasible region is only a point
                 */
                if(feasibleRegion.vertice.size() == 1)
                    continue;

                feasibleRegion.intersectOfHalfPlane(c);
            }
            else{

                /**
                 *  if current feasible region has intersection with the new half plane,
                 *  then the optimal vertex must be on the boundary of the halfplane.
                 *
                 *  otherwise, the LP probelm is infeasible.
                 */

                updateOptimalSolution(feasibleRegion, c, ans);
            }
        }
    }

    /**
     * if the optimal solution is the boundary of original plane, then the LP has no solution.
     * It is unbounded
     */
    statusLP s = ans.getStatus();
    if(s == singlePoint){
        vertex vp(ans.getPoint());
        if(equal(vp.x,UPBOUND)||
                equal(vp.x, LOWBOUND)||
                equal(vp.y, UPBOUND)||
                equal(vp.y,LOWBOUND))
            ans.setSolution(unbounded);
    }

    return ans;
}

/**
 * @brief TDLP::updateOptimalSolution if c has intersection with the feasible region, then need to update the optimal solution
 *                                    otherwise, the LP has no feasible solution.
 * @param feasibleRegion
 * @param c
 * @param ans
 */
void TDLP::updateOptimalSolution(region& feasibleRegion, constraint& c, solution& ans)
{
    vertexSet optimalCandidateVertex;
    feasibleRegion.intersectOfHalfPlane(c, optimalCandidateVertex);

    size_t size = optimalCandidateVertex.size();

    /**
     * if half plane has no intersection with feasible region and the current optimal vertex is
     * not on the half plane, then the LP has no solution.
     */
    if(size == 0){
        ans.setStatus(noSolution);
        return;
    }

    /**
     * if the number of intersection is one, then the current optimal solution is a point
     */
    if(size == 1){
        double tmp = func.getValue(optimalCandidateVertex[0]);
        ans.setSolution(optimalCandidateVertex[0], tmp, singlePoint);
    }

    /**
     * if the number of intersection is two, then the current optimal solution is a line
     */
    if(size == 2){
        vertex vl(optimalCandidateVertex[0]);
        vertex vr(optimalCandidateVertex[1]);

        double vlFuncValue = func.getValue(vl);
        double vrFuncValue = func.getValue(vr);

        if(equal(vlFuncValue, vrFuncValue)){
            ans.setSolution(edge(vl,vr), vlFuncValue, singleLine);

        }else{
            double maxVal;
            vertex maxVertex;

            if(greaterThan(vlFuncValue, vrFuncValue)){
                maxVal = vlFuncValue;
                maxVertex = vl;
            }
            else{
                maxVal = vrFuncValue;
                maxVertex = vr;
            }

            if(equal(maxVal, ans.getFuncValue())){
                /**
                 * if current optimal value is equal to last optimal value, then the status of last optimal is single line
                 * and current optimal vertex is on that line
                 */
                edge e(ans.getEdge());
                if(c.isVertexOnHalfPlane(e.start))
                    ans.setSolution(edge(e.start, maxVertex), maxVal, singleLine);
                else
                    ans.setSolution(edge(maxVertex, e.end), maxVal, singleLine);
                return;
            }

            ans.setSolution(maxVertex, maxVal, singlePoint);
        }
    }

    return;
}


////////////////////////////////////////////////////////////
#if 0
ostream& operator << (ostream& out, const constraint& c)
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
#endif

ostream& operator << (ostream& out, const solution& ans)
{
    statusLP s = ans.getStatus();
    switch(s){
    case noSolution :
        out << "no feasible solution";
        break;
    case singlePoint :
        out << "status : the optimal solution is a point" << endl;
        out << "optimal solution : " << ans.getPoint() << endl;
        out << "objective value  : " << ans.getFuncValue();
        break;
    case singleLine :
        out << "status : the optimal solution is a line" << endl;
        out << "optimal solution : " << ans.getEdge() << endl;
        out << "objective value  : " << ans.getFuncValue();
        break;
    case unbounded :
        out << "status : the LP problem has no solution" << endl;
        out << "objective value : " << ans.getFuncValue();
        break;
    default :
        break;
    }
    return out;
}

ostream& operator << (ostream& out, const TDLP& tdlp)
{
    const constraintSet constraints = tdlp.getConstraint();
    const objFunc func = tdlp.getObjFunc();

    /**
     *  output the constaints
     */
    constConstraintIterator it;
    out << "constraints : " << endl;
    for(it = constraints.begin();it != constraints.end();it++){
        out << *it << endl;
    }

    /**
     * output the objective function
     */
    char* fmt = new char [200];

    sprintf(fmt, "max f= max ");

    if(!equal(func.xCoef, 0))
        sprintf(fmt, "%s%lfx", fmt, func.xCoef);

    if(greaterThan(func.yCoef, 0))
        sprintf(fmt, "%s+%lfy", fmt, func.yCoef);

    if(lessThan(func.yCoef, 0))
        sprintf(fmt, "%s-%lfy", fmt, -1*func.yCoef);

    out << "objective function : " << fmt;

    delete [] fmt;

    return out;
}
