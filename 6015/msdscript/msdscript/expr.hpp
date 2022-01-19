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
    virtual int interp()=0;
    virtual bool has_variable()=0;
    virtual Expr* subst(std::string original, Expr* replacement)=0;
};

class Num : public Expr {
public:
    int val;
    
    Num(int val);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
};

class Var : public Expr{
public:
    std::string variable;
    
    Var(std::string variable);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
};


#endif /* expr_hpp */
