#ifndef TDLP_H
#define TDLP_H

#include <vector>
#include "base.h"

typedef class vertex point;
typedef class halfPlane constraint;

/**
 * @brief The stauts enum define the all posible condition of linear programming
 */
enum stauts{
    noSolution = 0,
    unbounded,
    line,
    vertex
};

/**
 * @brief The objective struct the objective function f = xCoef*x+yCoef*y
 */
class objFunc{
public :
    double xCoef;
    double yCoef;

    objFunc(){}

    objFunc(double xCoef, double yCoef) :
        xCoef(xCoef), yCoef(yCoef){}

    objFunc(const objFunc& obj){
        xCoef = obj.xCoef;
        yCoef = obj.yCoef;
    }

    inline double getValue(const point& v){
        return xCoef*v.x+yCoef*v.y;
    }
};

/**
 * @brief The solution struct the optimal feasible solution of two dimentional incremental random linear programming
 */
class solution{
public :
    double x;
    double y;
    double funcValue;

    solution(){}

    solution(double x, double y, double funcValue) :
        x(x),y(y),funcValue(funcValue)
    {}

    solution(const point& v, double funcValue) :
        x(v.x),y(v.y),funcValue(funcValue)
    {}

    solution(const solution& s) :
        x(s.x),y(s.y),funcValue(s.funcValue)
    {}
};

/**
 * @brief The TDLP class
 */
class TDLP
{
public:
    TDLP();
    TDLP(const TDLP& tdlp);

    TDLP& operator = (const TDLP& tdlp);

    void addConstraint(const constraint& c);
    void delConstraint(const constraint& c);
    void setObjFunc(const objFunc& f);

    solution solve();

private :
    vector<constraint> constraints;
    objFunc func;
};


ostream& operator << (ostream& out, const solution& ans);
ostream& operator << (ostream& out, const TDLP& tdlp);

#endif // TDLP_H
