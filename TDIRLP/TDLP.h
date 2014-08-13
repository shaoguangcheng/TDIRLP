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
    singleLine,
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

    string toString() const
    {
        char* fmt = new char [200];

        sprintf(fmt, "max f= max ");

        if(!equal(xCoef, 0))
            sprintf(fmt, "%s%lfx", fmt, xCoef);

        if(greaterThan(yCoef, 0))
            sprintf(fmt, "%s+%lfy", fmt, yCoef);

        if(lessThan(yCoef, 0))
            sprintf(fmt, "%s-%lfy", fmt, -1*yCoef);

        string s(fmt);

        delete [] fmt;

        return s;
    }

    inline void clear(){
        xCoef = 0;
        yCoef = 0;
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
    point optimalPoint;
    edge optimalEdge;

    double funcValue;
    statusLP status;

public :
    solution(){}

    solution(const point& v, double funcValue) :
        optimalPoint(v),funcValue(funcValue), status(singlePoint)
    {
        format();
    }

    solution(const edge& e, double funcValue) :
        optimalEdge(e), funcValue(funcValue), status(singleLine)
    {
        format();
    }

    solution(const solution& s) :
        status(s.getStatus())
    {
        if(status == singlePoint){
            funcValue = s.getFuncValue();
            optimalPoint = s.getPoint();
        }

        if(status == singleLine){
            funcValue = s.getFuncValue();
            optimalEdge  = s.getEdge();
        }

        if(status == unbounded){
            funcValue = s.getFuncValue();
        }

        format();
    }

    solution& operator = (const solution& s)
    {
        status = s.getStatus();

        if(status == singlePoint){
            funcValue = s.getFuncValue();
            optimalPoint = s.getPoint();
        }

        if(status == singleLine){
            funcValue = s.getFuncValue();
            optimalEdge  = s.getEdge();
        }

        if(status == unbounded){
            funcValue = s.getFuncValue();
        }

        format();

        return *this;
    }

    void setSolution(const point& p, double value, statusLP s)
    {
        if(s == singlePoint){
            optimalPoint = p;
            funcValue = value;
            status = s;

            format();
        }
        else{
            DEBUGMSG("solution status can not match");
            exit(EXIT_FAILURE);
        }
    }

    void setSolution(const edge& e, double value, statusLP s)
    {
        if(s == singleLine){
            optimalEdge = e;
            funcValue   = value;
            status = s;

            format();
        }
        else{
            DEBUGMSG("solution status can not match");
            exit(EXIT_FAILURE);
        }
    }

    void format()
    {
        if(status == singlePoint){
            funcValue = setPrecision(funcValue, 2);
            optimalPoint.x = setPrecision(optimalPoint.x, 2);
            optimalPoint.y = setPrecision(optimalPoint.y, 2);

            funcValue = setPrecision(funcValue, 2);
        }

        if(status == singleLine){
            funcValue = setPrecision(funcValue, 2);
            optimalEdge.start.x = setPrecision(optimalEdge.start.x, 2);
            optimalEdge.start.y = setPrecision(optimalEdge.start.y, 2);
            optimalEdge.end.x = setPrecision(optimalEdge.end.x, 2);
            optimalEdge.end.y = setPrecision(optimalEdge.end.y, 2);

            funcValue = setPrecision(funcValue, 2);
        }
    }

    void setSolution(statusLP s)
    {
        if(s == unbounded){
            funcValue   = numeric_limits<double>::infinity();
            status = s;
        }
        else if(s == noSolution){
            status = s;
        }else{
            DEBUGMSG("solution status can not match");
            exit(EXIT_FAILURE);
        }
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
        if(status == singlePoint){
            return optimalPoint;
        }
        else{
            DEBUGMSG("solution status can not match");
            exit(EXIT_FAILURE);
        }
    }

    inline edge getEdge() const
    {
        if(status == singleLine){
            return optimalEdge;
        }
        else{
            DEBUGMSG("solution status can not match");
            exit(EXIT_FAILURE);
        }
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
    ~TDLP();

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
    void clear();

private :
    void updateOptimalSolution(region& feasibleRegion, constraint &c, solution& ans);

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

#if 0
ostream& operator << (ostream& out, const constraint& c);
#endif
ostream& operator << (ostream& out, const solution& ans);
ostream& operator << (ostream& out, const TDLP& tdlp);

#endif // TDLP_H
