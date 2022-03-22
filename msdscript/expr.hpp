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
#include "pointer.h"
#include "env.hpp"

class Val; //declaration of calss Val so that declaration of
            //Expr::interp can refer to Val*

typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult,      // = 2
  prec_let        // = 3
} precedence_t;

CLASS(Expr){
public:
    virtual ~Expr() {};
    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val) interp(PTR(Env) env)=0;
    virtual bool has_variable()=0;
    //virtual PTR(Expr) subst(std::string original, PTR(Expr) replacement)=0;
    virtual void print(std::ostream &out) = 0;
    virtual void pretty_print(std::ostream &out)=0;
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed) = 0;
    static std::string to_string(PTR(Expr) e);
    static std::string to_string_pretty(PTR(Expr) e);
    std::string to_string_cmd();
    std::string to_pretty_string_cmd();
};

//subclass
//class NumExpr : public Expr{
//    int rep;
//    NumExpr(int rep){
//        this->rep =rep;
//    }
//};

class NumExpr : public Expr {
public:
    int val;
    
    NumExpr(int val);

    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    //PTR(Expr) subst(std::string original, PTR(Expr) replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
    
};

class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    //PTR(Expr) subst(std::string original, PTR(Expr) replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};

class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    //PTR(Expr) subst(std::string original, PTR(Expr) replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};

class VarExpr : public Expr{
public:
    std::string variable;
    
    VarExpr(std::string variable);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    //PTR(Expr) subst(std::string original, PTR(Expr) replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};


class _letExpr : public Expr{
public:
    std::string _letVariable;
    PTR(Expr) rhs;
    PTR(Expr) body;
    
    _letExpr(std::string _letVariable, PTR(Expr) rhs, PTR(Expr) body);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    //PTR(Expr) subst(std::string original, PTR(Expr) replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};

class EqualExpr : public Expr {
public:
  PTR(Expr) lhs;
  PTR(Expr) rhs;
   
  EqualExpr(PTR(Expr) lhs, PTR(Expr) rhs);
  bool equals(PTR(Expr) e);
  PTR(Val) interp(PTR(Env) env);
  bool has_variable();
  //PTR(Expr) subst(std::string str, PTR(Expr) e);
  void print(std::ostream &out);
  void pretty_print(std::ostream &out);
  void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

class BoolExpr : public Expr {
public:
  bool boolean;
   
  BoolExpr(bool val);
  bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
  bool has_variable();
    //PTR(Expr) subst(std::string str, PTR(Expr) e);
  void print(std::ostream &out);
  void pretty_print(std::ostream &out);
  void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

class IfExpr : public Expr {
public:
    PTR(Expr) _if;
    PTR(Expr) _then;
    PTR(Expr) _else;
   
  IfExpr(PTR(Expr)_if, PTR(Expr)_then, PTR(Expr)_else);
  bool equals(PTR(Expr) e);
  PTR(Val) interp(PTR(Env) env);
  bool has_variable();
  //PTR(Expr) subst(std::string str, PTR(Expr) e);
  void print(std::ostream &out);
  void pretty_print(std::ostream &out);
  void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

class FunExpr : public Expr{
public:
    std::string formal_arg;
    PTR(Expr) body;
    
    FunExpr(std::string formal_arg, PTR(Expr) body);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    //PTR(Expr) subst(std::string str, PTR(Expr) e);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int space);
    
};

class CallExpr : public Expr{
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    //PTR(Expr) subst(std::string str, PTR(Expr) e);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

#endif /* expr_hpp */
