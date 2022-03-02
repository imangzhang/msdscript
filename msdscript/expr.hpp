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

class Val; //declaration of calss Val so that declaration of
            //Expr::interp can refer to Val*

typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult,      // = 2
  prec_let        // = 3
} precedence_t;

class Expr{
public:
    virtual bool equals(Expr *e) = 0;
    virtual Val* interp()=0;
    virtual bool has_variable()=0;
    virtual Expr* subst(std::string original, Expr* replacement)=0;
    virtual void print(std::ostream &out) = 0;
    virtual void pretty_print(std::ostream &out)=0;
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed) = 0;
    static std::string to_string(Expr* e);
    static std::string to_string_pretty(Expr* e);
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

    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
    
};

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};

class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    MultExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};

class VarExpr : public Expr{
public:
    std::string variable;
    
    VarExpr(std::string variable);
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};


class _letExpr : public Expr{
public:
    std::string _letVariable;
    Expr *rhs;
    Expr *body;
    
    _letExpr(std::string _letVariable, Expr *rhs, Expr *body);
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string original, Expr* replacement);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed);
};

class EqualExpr : public Expr {
public:
  Expr *lhs;
  Expr *rhs;
   
  EqualExpr(Expr *lhs, Expr *rhs);
  bool equals(Expr *e);
  Val* interp();
  bool has_variable();
  Expr* subst(std::string str, Expr *e);
  void print(std::ostream &out);
  void pretty_print(std::ostream &out);
  void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

class BoolExpr : public Expr {
public:
  bool boolean;
   
  BoolExpr(bool val);
  bool equals(Expr *e);
  Val* interp();
  bool has_variable();
  Expr* subst(std::string str, Expr *e);
  void print(std::ostream &out);
  void pretty_print(std::ostream &out);
  void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

class IfExpr : public Expr {
public:
  Expr *_if;
  Expr *_then;
  Expr *_else;
   
  IfExpr(Expr *_if, Expr *_then, Expr *_else);
  bool equals(Expr *e);
  Val* interp();
  bool has_variable();
  Expr* subst(std::string str, Expr *e);
  void print(std::ostream &out);
  void pretty_print(std::ostream &out);
  void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

class FunExpr : public Expr{
public:
    std::string formal_arg;
    Expr *body;
    
    FunExpr(std::string formal_arg, Expr *body);
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string str, Expr *e);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int space);
    
};

class CallExpr : public Expr{
public:
    Expr* to_be_called;
    Expr* actual_arg;
    
    CallExpr(Expr *to_be_called, Expr *actual_arg);
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string str, Expr *e);
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t prec, int space);
};

#endif /* expr_hpp */
