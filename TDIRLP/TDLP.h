#ifndef TDLP_H
#define TDLP_H

#include <vector>
#include "base.h"
#include "util.h"

typedef class vertex point;
typedef class halfPlane constraint;
typedef class polygon region;
typedef vector<constraint> constraintSet;
typedef vector<constraint>::iterator constraintIterator;
typedef vector<constraint>::const_iterator constConstraintIterator;

/**
 * @brief The stauts enum define the all posible condition of linear programming
 */
enum statusLP{
    noSolution = 0,
    unbounded,
    line,
    singlePoint
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
private :
    double x;
    double y;
    double funcValue;
    statusLP status;

public :
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

    inline void setSolution(const point& p, double value, statusLP s)
    {
        x = p.x;
        y = p.y;
        funcValue = value;
        status = s;
    }

    inline void setStatus(statusLP s)
    {
        status = s;
    }

    inline double getFuncValue() const
    {
        if(status == noSolution){
            DEBUGMSG("Linear Programming problem has no feasible solution");
            exit(EXIT_FAILURE);
        }

        return funcValue;
    }

    inline point getPoint() const
    {
        if(status == noSolution){
            DEBUGMSG("Linear Programming problem has no feasible solution");
            exit(EXIT_FAILURE);
        }

        return point(x, y);
    }

    inline statusLP getStatus() const
    {
        return status;
    }
};

struct isConstraintIndentical : public binary_function <constraint, constraint, bool>
{
    /**
     * @brief operator () here must be careful : in order to define a correct functor, the overloaded
     *                                           operator ()  must be defined as const function
     * @param c1
     * @param c2
     * @return
     */
    bool operator () (const constraint& c1, const constraint& c2) const /*'const' can not be neglected*/{
        return equal(c1.xCoef, c2.xCoef)&&
                equal(c1.yCoef, c2.yCoef)&&
                equal(c1.bias, c2.bias);
    }
};

/**
 * another solution for compare constraint
 */
class _isConstraintIndentical_ : public unary_function <constraint, bool>
{
public :
    _isConstraintIndentical_(constraint c) :
        c(c)
    {}

    bool operator () (const constraint& c) const {
        return equal(this->c.xCoef, c.xCoef)&&
                equal(this->c.yCoef, c.yCoef)&&
                equal(this->c.bias, c.bias);
    }

private :
    constraint c;
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

    void addConstraint(const constraint &c);
    void delConstraint(const constraint &c);
    void setObjFunc(const objFunc& f);

    /**
     * @brief solve solve the TDIRLP problem
     * @return
     */
    solution solve();

    constraintSet getConstraint() const;
    objFunc getObjFunc() const;

private :
    /**
     * @brief constraints all constraints of LP
     */
    constraintSet constraints;

    /**
     * @brief func the objective function of LP
     */
    objFunc func;
};

ostream& operator << (ostream& out, const constraint& c);
ostream& operator << (ostream& out, const solution& ans);
ostream& operator << (ostream& out, const TDLP& tdlp);

#endif // TDLP_H
