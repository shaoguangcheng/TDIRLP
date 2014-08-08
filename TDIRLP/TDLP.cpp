#include "TDLP.h"

#include <stdio.h>
#include <algorithm>
#include <string.h>

TDLP::TDLP()
{
}

TDLP::TDLP(const TDLP &tdlp) :
    constraints(tdlp.getConstraint()),
    func(tdlp.getObjFunc())
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
//    random_shuffle(constraints.begin(), constraints.end());

    constConstraintIterator it;
    for(it = constraints.begin(); it != constraints.end(); it++){
        constraint c = *it;
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
             *  if current optimal vertex has intersection with the new half plane,
             *  then the optimal vertex must be on the boundary of the halfplane.
             *
             *  otherwise, the LP probelm is infeasible.
             */
            vertexSet optimalCandidateVertex;
            feasibleRegion.intersectOfHalfPlane(c, optimalCandidateVertex);


            size_t size = optimalCandidateVertex.size();

            if(size == 0){
                ans.setStatus(noSolution);
                return ans;
            }

            if(size == 1){
                double tmp = func.getValue(optimalCandidateVertex[0]);
                ans.setSolution(optimalCandidateVertex[0], tmp, singlePoint);
            }

            if(size == 2){
                vertex vl(optimalCandidateVertex[0]);
                vertex vr(optimalCandidateVertex[1]);

                double vlFuncValue = func.getValue(vl);
                double vrFuncValue = func.getValue(vr);
                if(equal(vlFuncValue, vrFuncValue)){
                    ans.setSolution(vl, vlFuncValue, line);
                }

                if(greaterThan(vlFuncValue, vrFuncValue)){
                    ans.setSolution(vl, vlFuncValue, singlePoint);
                }
                else{
                    ans.setSolution(vr, vrFuncValue, singlePoint);
                }
            }

        }
    }

    return ans;
}


////////////////////////////////////////////////////////////
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

ostream& operator << (ostream& out, const solution& ans)
{
    out << "optimal solution : " << ans.getPoint() << endl;
    out << "objective value  : " << ans.getFuncValue();
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
    char* fmt = new char [100];

    sprintf(fmt, "f=");

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
