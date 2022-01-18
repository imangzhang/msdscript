//
//  expr.hpp
//  msdscript
//
//  Created by Ang Zhang on 1/17/22.
//

#ifndef expr_hpp
#define expr_hpp

#include <stdio.h>
#include <string>

class Expr{
public:
    virtual bool equals(Expr *e) = 0;
};

class Num : public Expr {
public:
    int val;
    
    Num(int val);
    bool equals(Expr *e);
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
};

class Var : public Expr{
public:
    std::string variable;
    
    Var(std::string variable);
    bool equals(Expr *e);
};


#endif /* expr_hpp */
