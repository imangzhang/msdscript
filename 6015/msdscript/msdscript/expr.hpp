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

typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult       // = 2
} precedence_t;

class Expr{
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp()=0;
    virtual bool has_variable()=0;
    virtual Expr* subst(std::string original, Expr* replacement)=0;
    virtual void print(std::ostream &out) = 0;
    virtual void pretty_print(std::ostream &out)=0;
    virtual precedence_t pretty_print_at() = 0;
    static std::string to_string(Expr* e);
    static std::string to_string_pretty(Expr* e);
};

class Num : public Expr {
public:
    int val;
    
    Num(int val);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    void print(std::ostream &out);
    virtual void pretty_print(std::ostream &out);
    virtual precedence_t pretty_print_at();
    
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
    void print(std::ostream &out);
    virtual void pretty_print(std::ostream &out);
    virtual precedence_t pretty_print_at();
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
    void print(std::ostream &out);
    virtual void pretty_print(std::ostream &out);
    virtual precedence_t pretty_print_at();
};

class Var : public Expr{
public:
    std::string variable;
    
    Var(std::string variable);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    void print(std::ostream &out);
    virtual void pretty_print(std::ostream &out);
    precedence_t pretty_print_at();
};


#endif /* expr_hpp */
