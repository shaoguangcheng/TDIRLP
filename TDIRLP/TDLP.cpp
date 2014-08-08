#include "TDLP.h"
#include "util.h"

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
    out << "optimal solution : (" << ans.x << ", " << ans.y << ")" << endl;
    out << "objective value  : " << ans.funcValue;
    return out;
}

ostream& operator << (ostream& out, const TDLP& tdlp)
{
    const constraintSet constraints = tdlp.getConstraint();
    const objFunc func = tdlp.getObjFunc();

    constConstraintIterator it;
    out << "constraints : " << endl;
    for(it = constraints.begin();it != constraints.end();it++){
        out << *it << endl;
    }

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
